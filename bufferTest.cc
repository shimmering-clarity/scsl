#include <cassert>
#include <iostream>

#include "Buffer.h"
using namespace klib;


int
main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	Buffer buffer("hlo, world");
	Buffer helloWorld("hello, world!");
	Buffer goodbyeWorld("goodbye, world");
	Buffer goodbyeCruelWorld("goodbye, cruel world");

	std::cout << buffer << std::endl;

	buffer.Insert(1, (uint8_t *) "el", 2);
	buffer.Append('!');
	assert(buffer == helloWorld);

	std::cout << buffer << std::endl;

	buffer.Remove(buffer.Length() - 1);
	std::cout << buffer << std::endl;
	buffer.Remove(0, 5);
	std::cout << buffer << std::endl;
	buffer.Insert(0, 'g');
	std::cout << buffer << std::endl;
	buffer.Insert(1, (uint8_t *) "oodbye", 6);
	std::cout << buffer << std::endl;
	assert(buffer == goodbyeWorld);
	buffer.Insert(9, (uint8_t *)"cruel ", 6);

	std::cout << buffer << std::endl;
	buffer.HexDump(std::cout);

	buffer.Reclaim();
	buffer.Append("and now for something completely different...");

	std::cout << buffer.Contents() << std::endl;
	std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;
	buffer.Resize(128);
	std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;
	buffer.Trim();
	std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;

	Buffer	buffer2("and now for something completely different...");
	assert(buffer == buffer2);

	buffer2.Remove(buffer2.Length()-3, 3);
	std::cout << buffer << std::endl;
	assert(buffer != buffer2);

	return 0;
}
