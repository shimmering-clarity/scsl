#include <iostream>

#include "Buffer.h"


int
main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	klib::Buffer buffer("hlo, world");

	std::cout << buffer.Contents() << std::endl;

	buffer.Insert(1, (uint8_t *) "el", 2);
	buffer.Append('!');

	std::cout << buffer.Contents() << std::endl;

	std::cout << "remove end" << std::endl;
	buffer.Remove(buffer.Length() - 1);

	std::cout << "remove start" << std::endl;
	buffer.Remove(0, 5);

	std::cout << "insert char" << std::endl;
	buffer.Insert(0, 'g');

	std::cout << "insert chunk" << std::endl;
	buffer.Insert(1, (uint8_t *) "oodbye", 6);
	std::cout << "cruel" << std::endl;
	buffer.Insert(9, (uint8_t *)"cruel ", 6);
	std::cout << buffer.Contents() << std::endl;
	buffer.HexDump(std::cout);

	std::cout << "reclaim" << std::endl;
	buffer.Reclaim();

	std::cout << "append" << std::endl;
	buffer.Append("and now for something completely different...");

	std::cout << buffer.Contents() << std::endl;
	std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;
	buffer.Resize(128);
	std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;
	buffer.Trim();
	std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;


	return 0;
}
