#include "../include/icon7-godot-client/ByteWriter.hpp"

#define METHOD_NO_ARGS(CLASS, NAME)                                            \
	godot::ClassDB::bind_method(godot::D_METHOD(#NAME), &CLASS::NAME);

#define METHOD_ARGS(CLASS, NAME, ...)                                          \
	godot::ClassDB::bind_method(godot::D_METHOD(#NAME, __VA_ARGS__),           \
								&CLASS::NAME);

GodotByteWriter::GodotByteWriter() : byteWriter(data) {}

void GodotByteWriter::_bind_methods()
{
	METHOD_ARGS(GodotByteWriter, WriteInt8, "value");
	METHOD_ARGS(GodotByteWriter, WriteInt16, "value");
	METHOD_ARGS(GodotByteWriter, WriteInt32, "value");
	METHOD_ARGS(GodotByteWriter, WriteInt64, "value");
	METHOD_ARGS(GodotByteWriter, WriteFloat32, "value");
	METHOD_ARGS(GodotByteWriter, WriteFloat64, "value");
	METHOD_ARGS(GodotByteWriter, WriteString, "value");

	METHOD_NO_ARGS(GodotByteWriter, GetBytes);
	METHOD_ARGS(GodotByteWriter, Reserve, "capacity");
}

GodotByteWriter *GodotByteWriter::WriteInt8(int64_t v)
{
	byteWriter.op((uint8_t)v);
	this->reference();
	return this;
}

GodotByteWriter *GodotByteWriter::WriteInt16(int64_t v)
{
	byteWriter.op((uint16_t)v);
	this->reference();
	return this;
}

GodotByteWriter *GodotByteWriter::WriteInt32(int64_t v)
{
	byteWriter.op((uint32_t)v);
	this->reference();
	return this;
}

GodotByteWriter *GodotByteWriter::WriteInt64(int64_t v)
{
	byteWriter.op(v);
	this->reference();
	return this;
}

GodotByteWriter *GodotByteWriter::WriteFloat32(double_t v)
{
	byteWriter.op((float)v);
	this->reference();
	return this;
}

GodotByteWriter *GodotByteWriter::WriteFloat64(double_t v)
{
	byteWriter.op((double)v);
	this->reference();
	return this;
}

GodotByteWriter *GodotByteWriter::WriteString(const godot::String &v)
{
	auto utf8 = v.utf8();
	std::string_view sv(utf8.ptr(), utf8.length());
	byteWriter.op(sv);
	this->reference();
	return this;
}

int64_t GodotByteWriter::GetBytes() const { return data.size(); }

void GodotByteWriter::Reserve(int64_t capacity) { data.reserve(capacity); }
