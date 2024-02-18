#include "../include/icon7-godot-client/ByteReader.hpp"

#define METHOD_NO_ARGS(CLASS, NAME)                                            \
	godot::ClassDB::bind_method(godot::D_METHOD(#NAME), &CLASS::NAME);

#define METHOD_ARGS(CLASS, NAME, ...)                                          \
	godot::ClassDB::bind_method(godot::D_METHOD(##NAME, __VA_ARGS__),          \
								&CLASS::NAME);

void GodotByteReader::_bind_methods()
{
	METHOD_NO_ARGS(GodotByteReader, GetInt8);
	METHOD_NO_ARGS(GodotByteReader, GetInt16);
	METHOD_NO_ARGS(GodotByteReader, GetInt32);
	METHOD_NO_ARGS(GodotByteReader, GetInt64);
	METHOD_NO_ARGS(GodotByteReader, GetFloat32);
	METHOD_NO_ARGS(GodotByteReader, GetFloat64);
	METHOD_NO_ARGS(GodotByteReader, GetString);

	METHOD_NO_ARGS(GodotByteReader, GetBytesLeft);
	METHOD_NO_ARGS(GodotByteReader, GetOffset);
	METHOD_NO_ARGS(GodotByteReader, GetTotalBytes);
}

int64_t GodotByteReader::GetInt8()
{
	uint8_t v;
	byteReader->op(v);
	return v;
}

int64_t GodotByteReader::GetInt16()
{
	uint16_t v;
	byteReader->op(v);
	return v;
}

int64_t GodotByteReader::GetInt32()
{
	uint32_t v;
	byteReader->op(v);
	return v;
}

int64_t GodotByteReader::GetInt64()
{
	uint64_t v;
	byteReader->op(v);
	return v;
}

double_t GodotByteReader::GetFloat32()
{
	float v;
	byteReader->op(v);
	return v;
}

double_t GodotByteReader::GetFloat64()
{
	double v;
	byteReader->op(v);
	return v;
}

godot::String GodotByteReader::GetString()
{
	std::string_view sv;
	byteReader->op(sv);

	godot::String str;
	str.utf8(sv.data(), sv.length());
	return str;
}

int64_t GodotByteReader::GetBytesLeft()
{
	return byteReader->get_remaining_bytes();
}

int64_t GodotByteReader::GetOffset() { return byteReader->get_offset(); }

int64_t GodotByteReader::GetTotalBytes() { return byteReader->_data.size(); }
