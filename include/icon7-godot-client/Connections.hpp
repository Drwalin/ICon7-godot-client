#ifndef ICON7_GODOT_CLIENT_RPC_CLIENT_HPP
#define ICON7_GODOT_CLIENT_RPC_CLIENT_HPP

#include <cstdint>

#include <memory>

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#include <icon7/Peer.hpp>
#include <icon7/Flags.hpp>
#include <icon7/RPCEnvironment.hpp>
#include <icon7/Flags.hpp>
#include <icon7/Command.hpp>
#include <icon7/PeerUStcp.hpp>
#include <icon7/HostUStcp.hpp>

#include "ByteReader.hpp"
#include "ByteWriter.hpp"

class RpcFlags : public godot::Object
{
	GDCLASS(RpcFlags, Object)
public:
	static void _bind_methods();
};

class RpcClient;

// this should be used as godot singleton object
class RpcHost : public godot::Node
{
	GDCLASS(RpcHost, Node)
public:
	static void _bind_methods();

	void RegisterMethod(const godot::String &funcName,
						const godot::Callable &callback);

	void Listen(const godot::String &interfaceAddress, int64_t port, const godot::Callable &onListen);

	void Connect(const godot::String &address, int64_t port,
				 const godot::Callable &onConnect);

	void _process(double_t delta) override;
	void _enter_tree() override;
	void _exit_tree() override;
	void _notification(int what);
	
	void _InternalDestroy();

	friend class RpcClient;

public:
	icon7::Host *host;
	icon7::RPCEnvironment rpc;
	icon7::CommandExecutionQueue executionQueue;

	std::set<RpcClient *> connections;
};

class RpcClient : public godot::Object
{
	GDCLASS(RpcClient, Object)

public:
	static void _bind_methods();
	
	void _notification(int what);

	void Disconnect();

	void Send(const godot::String &funcName, uint64_t flags,
			  const godot::PackedByteArray &data);
	void SendPrepared(uint64_t flags, GodotByteWriter *writer);
	// call with return is not implemented
	void Call(const godot::String &funcName, uint64_t flags,
			  const godot::PackedByteArray &data,
			  const godot::Callable &onConnect, const godot::Callable &onError);

	GDExtensionBool IsReadyToUse() const;
	GDExtensionBool IsDisconnecting() const;
	GDExtensionBool IsClosed() const;

	friend class RpcHost;

public:
	std::shared_ptr<icon7::Peer> peer;
	icon7::Host *host;
	icon7::RPCEnvironment *rpc;
	RpcHost *rpcHost;
};

#endif
