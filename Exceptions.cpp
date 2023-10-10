//
// Created by kyle on 2023-10-10.
//

#include "Exceptions.h"


namespace klib {


AssertionFailed::AssertionFailed(std::string message) : msg(message)
{
}

char *
AssertionFailed::what()
{
	return const_cast<char *>(this->msg.c_str());
}




}