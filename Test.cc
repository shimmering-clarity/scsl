//
// Created by kyle on 2023-10-09.
//

#include "Test.h"


#include <iostream>
#include <cassert>


namespace klib {


void
TestAssert(bool condition, std::string message = "Assertion failed.")
{
#if defined(NDEBUG)
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


AssertionFailed::AssertionFailed(std::string message) : msg(message)
{
}

char *
AssertionFailed::what() 
{
	return const_cast<char *>(this->msg.c_str());
}


} // namespace klib
