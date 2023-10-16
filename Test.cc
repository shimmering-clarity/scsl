//
// Created by kyle on 2023-10-09.
//

#include "Exceptions.h"
#include "Test.h"

#include <cassert>
#include <iostream>
#include <sstream>


namespace scsl {

void
TestAssert(bool condition, std::string message)
{
#if defined(NDEBUG) || defined(SCSL_NOEXCEPT)
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


void
TestAssert(bool condition)
{
#if defined(NDEBUG)
	if (condition) {
		return;
	}
#if defined(SCSL_NOEXCEPT)
	std::cerr << "Assertion failed!\n";
#else
	std::stringstream msg;

	msg << "assertion failed at " << __FILE__ << ":" << __LINE__;
	throw AssertionFailed(msg.str());
#endif
#else
	assert(condition);
#endif
}


} // namespace scsl
