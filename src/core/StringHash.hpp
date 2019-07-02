#pragma once

namespace ari
{
	class StringHash
	{
	public:
		template <unsigned N, unsigned I>
		struct HashHelper
		{
			constexpr static unsigned Calculate(const char(&str)[N])
			{
				return (HashHelper<N, I - 1>::Calculate(str) ^ (str[I - 1] & 0xFF)) * StringHash::PRIME;
			}
		};

		template <unsigned N>
		struct HashHelper<N, 1>
		{
			constexpr static unsigned Calculate(const char(&str)[N])
			{
				return (StringHash::OFFSET ^ (str[0] & 0xFF)) * StringHash::PRIME;
			}
		};

		template<unsigned N>
		constexpr static unsigned StaticHash(const char(&str)[N])
		{
			return HashHelper<N, N>::Calculate(str);
		}

		static const unsigned OFFSET = 0x01234567;
		static const unsigned PRIME = 0x89ABCDEF;
	};

} // namespace ari
