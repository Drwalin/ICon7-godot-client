#ifndef ICON7_GODOT_CLIENT_BYTE_WRITER_HPP
#define ICON7_GODOT_CLIENT_BYTE_WRITER_HPP

#include <cstdint>

#include <memory>

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#include "../../ICon7/include/icon7/ByteWriter.hpp"

class GodotByteWriter : public godot::RefCounted
{
	GDCLASS(GodotByteWriter, Object)
public:
	GodotByteWriter();
		
	static void _bind_methods();
	
	GodotByteWriter *WriteInt8(int64_t v);
	GodotByteWriter *WriteInt16(int64_t v);
	GodotByteWriter *WriteInt32(int64_t v);
	GodotByteWriter *WriteInt64(int64_t v);
	GodotByteWriter *WriteFloat32(double_t v);
	GodotByteWriter *WriteFloat64(double_t v);
	GodotByteWriter *WriteString(const godot::String &v);
	
	int64_t GetBytes() const;
	void Reserve(int64_t capacity);
	
public:
	std::vector<uint8_t> data;
	bitscpp::ByteWriter byteWriter;
};

#endif
