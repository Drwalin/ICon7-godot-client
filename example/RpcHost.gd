extends RpcHost;
class_name ICon7Rpc;

var peer: RpcClient;

func Sum(peer: RpcClient, flags:int, data: GodotByteReader)->void:
	var a:int = data.GetInt32();
	var b:int = data.GetInt32();
	print("Sum: ", a, " + ", b, " = ", a + b);
	pass;

func OnConnected(peer: RpcClient)->void:
	print("connected Peer: ", peer);
	self.peer = peer;
	pass;

func OnListen(listening: bool)->void:
	print("Is listening" if listening else "Is not listening");

func _ready()->void:
	print("_ready");
	RegisterMethod("sum", Sum);
	print("_ready 0.5");
	Listen(12345, OnListen);
	print("_ready1");
	Connect("127.0.0.1", 12345, self.OnConnected);
	print("_ready2");
	pass;
	
