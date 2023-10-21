///
/// \file include/sctest/Exceptions.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-10
/// \brief Custom exceptions for use in SCSL used in writing test programs.
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

#ifndef SCSL_EXCEPTIONS_H
#define SCSL_EXCEPTIONS_H


#include <exception>
#include <string>


namespace sctest {


/// \brief Exception reserved for unsupported platforms.
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


} // namespace sctest


#endif // SCSL_EXCEPTIONS_H
