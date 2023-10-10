//
// Created by kyle on 2023-10-09.
//

#include <string>

#ifndef KLIB_TEST_H
#define KLIB_TEST_H


namespace klib {


void
TestAssert(bool condition, std::string message);


class AssertionFailed : public std::exception {
public:
	explicit AssertionFailed(std::string message);

	char *what();

public:
	std::string msg;
};


} // namespace klib

#endif //KLIB_TEST_H
