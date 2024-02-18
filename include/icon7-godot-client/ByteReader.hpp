#ifndef ICON7_GODOT_CLIENT_BYTE_READER_HPP
#define ICON7_GODOT_CLIENT_BYTE_READER_HPP

#include <cstdint>

#include <memory>

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#include "../../ICon7/include/icon7/ByteReader.hpp"

class GodotByteReader : public godot::RefCounted
{
	GDCLASS(GodotByteReader, Object)
public:
	static void _bind_methods();
	
	int64_t GetInt8();
	int64_t GetInt16();
	int64_t GetInt32();
	int64_t GetInt64();
	double_t GetFloat32();
	double_t GetFloat64();
	godot::String GetString();
	
	int64_t GetBytesLeft();
	int64_t GetOffset();
	int64_t GetTotalBytes();
	
public:
	icon7::ByteReader *byteReader;
};

#endif
