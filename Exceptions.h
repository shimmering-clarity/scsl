//
// Created by kyle on 2023-10-10.
//

#ifndef KLIB_EXCEPTIONS_H
#define KLIB_EXCEPTIONS_H


#include <exception>
#include <string>

namespace klib {


class NotImplemented : public std::exception {
public:
	explicit NotImplemented(const char *pl) : platform((char *)pl) {}
	char *what() {
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
	char *what();

private:
	std::string msg;
};


} // namespace klib


#endif //KLIB_EXCEPTIONS_H
