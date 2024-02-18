#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>

#include <icon7/Command.hpp>
#include <icon7/Flags.hpp>
#include <icon7/Host.hpp>

#include "../include/icon7-godot-client/Connections.hpp"
#include "../include/icon7-godot-client/ByteReader.hpp"

#define METHOD_NO_ARGS(CLASS, NAME) \
	godot::ClassDB::bind_method( \
		godot::D_METHOD(#NAME), \
		&CLASS::NAME);

#define METHOD_ARGS(CLASS, NAME, ...) \
	godot::ClassDB::bind_method( \
		godot::D_METHOD(#NAME, __VA_ARGS__), \
		&CLASS::NAME);

void RpcFlags::_bind_methods()
{
	godot::ClassDB::bind_integer_constant("RpcFlags", "Flags", "UNRELIABLE",
										  icon7::FLAG_UNRELIABLE, true);
	godot::ClassDB::bind_integer_constant("RpcFlags", "Flags", "RELIABLE",
										  icon7::FLAG_RELIABLE, true);
}

void RpcHost::_bind_methods()
{
	METHOD_ARGS(RpcHost, RegisterMethod, "funcName", "callback");
	METHOD_ARGS(RpcHost, Connect, "address", "port", "onConnected");
	METHOD_ARGS(RpcHost, Listen, "port", "onListen");
}

void RpcHost::RegisterMethod(const godot::String &funcName,
							 const godot::Callable &callback)
{
	std::function<void(icon7::Peer *, icon7::ByteReader *, icon7::Flags)> func =
		[callback](icon7::Peer *peer, icon7::ByteReader *_bytes,
				   icon7::Flags flags) {
			RpcClient *rpcClient = (RpcClient *)(peer->userPointer);
			GodotByteReader *reader = memnew(GodotByteReader());
			reader->byteReader = _bytes;
			int64_t _flags = flags;
			callback.call(rpcClient, _flags, reader);
			reader->unreference();
		};
	rpc.RegisterMessage(funcName.utf8().ptr(), func, &executionQueue);
}

void RpcHost::Connect(const godot::String &address, int64_t port,
					  const godot::Callable &onConnect)
{
	RpcClient *con = memnew(RpcClient());
	con->host = host;
	con->rpc = &rpc;
	con->rpcHost = this;

	icon7::commands::ExecuteOnPeer com;
	com.data.resize(sizeof(godot::Callable));
	new (com.data.data()) godot::Callable(onConnect);
	com.userPointer = con;
	com.function = [](icon7::Peer *peer, auto bytes, void *_con) {
		RpcClient *con = (RpcClient *)_con;
		godot::Callable *onConnect = (godot::Callable *)(bytes.data());

		if (peer == nullptr) {
			delete con;
			con = nullptr;
		} else {
			con->peer = peer->shared_from_this();
			peer->userPointer = con;
			con->rpcHost->connections.insert(con);
			peer->host->GetRpcEnvironment()->Send(peer, icon7::FLAG_UNRELIABLE,
					"sum", (uint32_t)0x10203040, (uint32_t)0x08070605);
		}

		onConnect->call(con);

		onConnect->~Callable();
	};

	host->Connect(address.utf8().ptr(), port, std::move(com), &executionQueue);
}

void RpcHost::_process(double_t delta)
{
	while (executionQueue.HasAny()) {
		const uint32_t MAX_EVENTS = 128;
		icon7::Command commands[MAX_EVENTS];
		const uint32_t dequeued =
			executionQueue.TryDequeueBulkAny(commands, MAX_EVENTS);

		for (uint32_t i = 0; i < dequeued; ++i) {
			commands[i].Execute();
			commands[i].~Command();
		}
	}
}

void RpcHost::_enter_tree()
{
	icon7::Initialize();

	auto _host = std::make_shared<icon7::uS::tcp::Host>();
	_host->Init();
	host = _host;
	host->SetRpcEnvironment(&rpc);
	host->RunAsync();
}

void RpcHost::Listen(int64_t port, const godot::Callable &onListen)
{
	icon7::commands::ExecuteBooleanOnHost com;
	godot::Callable *c = new godot::Callable(onListen);
	com.userPointer = c;
	com.function = [](auto, bool result, void *data) {
		godot::Callable *c = (godot::Callable *)data;
		c->call((GDExtensionBool)true);
		delete c;
	};
	host->ListenOnPort(port, icon7::IPv4, std::move(com), &executionQueue);
}

void RpcHost::_exit_tree()
{
	host->DisconnectAllAsync();
	host->WaitStopRunning();
	host = nullptr;
	icon7::Deinitialize();
}

void RpcClient::_bind_methods()
{
	METHOD_NO_ARGS(RpcClient, Disconnect);
	METHOD_ARGS(RpcClient, Send, "functionName", "flags", "data");
	METHOD_ARGS(RpcClient, SendPrepared, "flags", "writer");
	METHOD_NO_ARGS(RpcClient, IsReadyToUse);
	METHOD_NO_ARGS(RpcClient, IsDisconnecting);
	METHOD_NO_ARGS(RpcClient, IsClosed);
}

void RpcClient::Disconnect()
{
	peer->Disconnect();
	peer->userPointer = nullptr;
	peer = nullptr;
	rpcHost->connections.erase(this);
	// TODO: queue this object to free
}

void RpcClient::Send(const godot::String &funcName, uint64_t flags,
					 const godot::PackedByteArray &data)
{
	std::vector<uint8_t> buf;
	auto fns = funcName.utf8();
	buf.resize(fns.size() + data.size());
	memcpy(buf.data(), fns.ptr(), fns.size());
	memcpy(buf.data() + fns.size(), data.ptr(), data.size());
	peer->Send(std::move(buf), icon7::Flags(flags) | icon7::FLAGS_CALL_NO_FEEDBACK);
}

void RpcClient::SendPrepared(uint64_t flags, GodotByteWriter *writer)
{
	writer->byteWriter.~ByteWriter();
	
	std::vector<uint8_t> buf;
	std::swap(buf, writer->data);
	
	peer->Send(std::move(buf), flags | icon7::FLAGS_CALL_NO_FEEDBACK);
	
	new (&(writer->byteWriter)) bitscpp::ByteWriter(writer->data);
}

GDExtensionBool RpcClient::IsReadyToUse() const
{
	if (peer && peer != nullptr) {
		return peer->IsReadyToUse();
	}
	return false;
}

GDExtensionBool RpcClient::IsDisconnecting() const
{
	if (peer && peer != nullptr) {
		return peer->IsDisconnecting();
	}
	return true;
}

GDExtensionBool RpcClient::IsClosed() const
{
	if (peer && peer != nullptr) {
		return peer->IsClosed();
	}
	return true;
}
