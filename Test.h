///
/// \file Test.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-09
/// \brief Test.h implements basic testing tools.
///
#ifndef KLIB_TEST_H
#define KLIB_TEST_H

#include <string>


namespace klib {


/// TestAssert is a variant on the assert macro.
///
/// If NDEBUG is set, TestAssert will throw an exception if condition is false.
/// Otherwise, it calls assert after printing the message.
///
/// In addition to NDEBUG, KLIB_NO_ASSERT will suppress assertions.
///
/// \throws AssertionFailed
///
/// \param condition The condition to assert.
/// \param message The message that should be displayed if condition is false.
inline void	TestAssert(bool condition, std::string message);


/// AssertionFailed indicates that some invariant didn't hold.
class AssertionFailed : public std::exception {
public:
	/// AssertionFailed is constructed with a message describing what
	/// failed.
	explicit AssertionFailed(std::string message);

	/// what returns a message describing the exception.
	char *what();

private:
	std::string msg;
};


} // namespace klib

#endif //KLIB_TEST_H
