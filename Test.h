///
/// \file Test.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-09
/// \brief Test.h implements basic testing tools.
///
#ifndef SCSL_TEST_H
#define SCSL_TEST_H

#include <string>


namespace scsl {


/// TestAssert is a variant on the assert macro. This variant is intended to be
/// a drop-in replacement for the cassert macro: even in release mode, the tests
/// should still run.
///
/// If NDEBUG is set, TestAssert will throw an exception if condition is false.
/// Otherwise, it calls assert after printing the message.
///
/// \param condition If true, TestAssert throws an exception.
void	TestAssert(bool condition);


/// TestAssert is a variant on the assert macro.
///
/// If NDEBUG is set, TestAssert will throw an exception if condition is false.
/// Otherwise, it calls assert after printing the message.
///
/// In addition to NDEBUG, SCSL_NOEXCEPT will suppress assertions.
///
/// \throws AssertionFailed
///
/// \param condition The condition to assert.
/// \param message The message that should be displayed if condition is false.
void	TestAssert(bool condition, std::string message);


} // namespace scsl

#endif //SCSL_TEST_H
