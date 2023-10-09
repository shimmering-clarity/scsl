#include <iostream>

#include "Buffer.h"


int
main()
{
	klib::Buffer	buffer("hlo, world");

	std::cout << buffer.Contents() << std::endl;

	buffer.Insert(1, (uint8_t *)"el", 2);

	std::cout << buffer.Contents() << std::endl;
}
