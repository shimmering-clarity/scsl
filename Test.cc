///
/// \file Test.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-09
/// \brief Tooling to assist in building test programs..
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that
/// the above copyright notice and this permission notice appear in all /// copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
/// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
/// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
/// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
/// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
/// PERFORMANCE OF THIS SOFTWARE.
///

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
