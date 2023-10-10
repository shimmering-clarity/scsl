//
// Created by kyle on 2023-10-09.
//

#include "Exceptions.h"
#include "Test.h"


#include <iostream>
#include <cassert>


namespace klib {


void
TestAssert(bool condition, std::string message = "Assertion failed.")
{
#if defined(NDEBUG) || defined(KLIB_NO_ASSERT)
	if (!condition) {
		throw AssertionFailed(message);
	}
#else
	if (!condition) {
		std::cerr << message << std::endl;
	}
	assert(condition);
#endif
}


} // namespace klib
