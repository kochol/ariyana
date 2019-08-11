#pragma once
#include <Meta.h>

namespace ari::net
{
	template<typename Class,
		typename Stream,
		typename = std::enable_if_t <meta::isRegistered<Class>()>>
	bool Serialize(Stream & stream, Class & obj);

	template <typename Class,
		typename Stream,
		typename = std::enable_if_t <!meta::isRegistered<Class>()>,
		typename = void>
	bool Serialize(Stream & stream, Class & obj);

	template <typename Class, typename Stream>
	bool SerializeBasic(Stream& stream, Class& obj);

} // namespace ari::net

#include "Serialize.inl"
