//
// Created by kyle on 2023-10-10.
//

#ifndef KLIB_EXCEPTIONS_H
#define KLIB_EXCEPTIONS_H


#include <exception>
#include <string>

namespace klib {


/// NotImplemented is an exception reserved for unsupported platforms.
///
/// It is used to mark functionality included for compatibility, and useful for
/// debugging.
class NotImplemented : public std::exception {
public:
	/// NotImplemented exceptions are constructed with a platform name.
	explicit NotImplemented(const char *pl) : platform((char *)pl) {}

	/// what returns a message naming the platform.
	const char *what() const throw() {
		return this->platform;
	}
private:
	char *platform;
};


/// AssertionFailed indicates that some invariant didn't hold.
class AssertionFailed : public std::exception {
public:
	/// AssertionFailed is constructed with a message describing what
	/// failed.
	explicit AssertionFailed(std::string message);

	/// what returns a message describing the exception.
	const char *what() const throw();

private:
	std::string msg;
};


} // namespace klib


#endif //KLIB_EXCEPTIONS_H
