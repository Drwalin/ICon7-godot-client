extends Node2D

var peer: RpcClient;

# Called when the node enters the scene tree for the first time.
func _ready()->void:
	pass # Replace with function body.

var sent:int = 0;

var time:float = 0;

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta:float)->void:
	peer = rpcHost.peer;
	if peer:
		time += delta;
		if sent < 5:
			var writer:GodotByteWriter = GodotByteWriter.new();
			writer.WriteString("sum");
			writer.WriteInt32(sent);
			writer.WriteInt32(100);
			peer.SendPrepared(RpcFlags.RELIABLE, writer);
			
			peer.SendPrepared(RpcFlags.RELIABLE, GodotByteWriter.new().WriteString("sum").WriteInt32(sent*7).WriteInt32(10000));
			
			var bytes:PackedByteArray;
			bytes.resize(8);
			bytes.encode_u32(0, sent*5);
			bytes.encode_u32(4, 1000);
			peer.Send("sum", RpcFlags.RELIABLE, bytes);
			sent += 1;
		else:
			if time > 1.0:
				get_tree().quit();
	pass;
