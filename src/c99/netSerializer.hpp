#pragma once

#include "yojimbo.h"

template <typename Stream>
bool SerializeFloat(Stream& _stream, float& _obj)
{
	serialize_float(_stream, _obj);
	return true;
}

template <typename Stream>
bool SerializeDouble(Stream& _stream, double& _obj)
{
	serialize_double(_stream, _obj);
	return true;
}

template <typename Stream>
bool SerializeUInt64(Stream& _stream, uint64_t& _obj)
{
	serialize_uint64(_stream, _obj);
	return true;
}

template <typename Stream>
bool SerializeBool(Stream& _stream, bool& _obj)
{
	serialize_bool(_stream, _obj);
	return true;
}

template <typename Stream>
bool SerializeString(Stream& _stream, char* _obj, int length)
{
	serialize_string(_stream, _obj, length);
	return true;
}

template <typename Stream>
bool SerializeBits(Stream& _stream, uint32_t& _obj, int bits)
{
	serialize_bits(_stream, _obj, bits);
	return true;
}

template <typename Stream>
bool SerializeBytes(Stream& _stream, void* _obj, int bytes)
{
	serialize_bytes(_stream, (uint8_t*)_obj, bytes);
	return true;
}
