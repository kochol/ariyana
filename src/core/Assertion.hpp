#pragma once

#include <assert.h>     // assert

#define a_assert(cond) assert(cond)
#define a_assert_range(val, max) assert((val >= 0) && (val < max))

#if FIPS_DEBUG
#   define a_assert_dbg(cond) assert(cond)
#	define a_assert_range_dbg(val, max) a_assert_range(val, max)
#else
#   define a_assert_dbg(cond)
#	define a_assert_range_dbg(val, max)
#endif // FIPS_DEBUG
