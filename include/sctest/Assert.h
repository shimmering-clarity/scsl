///
/// \file Assert.h
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

#ifndef SCSL_TEST_H
#define SCSL_TEST_H


#include <string>


namespace sctest {


/// Assert is a variant on the assert macro. This variant is intended to be
/// a drop-in replacement for the cassert macro: even in release mode, the tests
/// should still run.
///
/// If NDEBUG is set, Assert will throw an exception if condition is false.
/// Otherwise, it calls assert after printing the message.
///
/// \param condition If true, Assert throws an exception.
void	Assert(bool condition);


/// Assert is a variant on the assert macro.
///
/// If NDEBUG is set, Assert will throw an exception if condition is false.
/// Otherwise, it calls assert after printing the message.
///
/// In addition to NDEBUG, SCSL_NOEXCEPT will suppress assertions.
///
/// \throws AssertionFailed
///
/// \param condition The condition to assert.
/// \param message The message that should be displayed if condition is false.
void	Assert(bool condition, std::string message);


} // namespace scsl

#endif //SCSL_TEST_H
