#include <iostream>

#include "Buffer.h"


int
main(int argc, char *argv[])
{
	(void)argc; (void)argv;

	klib::Buffer	buffer("hlo, world");

	std::cout << buffer.Contents() << std::endl;

	buffer.Insert(1, (uint8_t *)"el", 2);
	buffer.Append('!');

	std::cout << buffer.Contents() << std::endl;

	buffer.Remove(buffer.Length()-1);
	buffer.Remove(0, 5);
	buffer.Insert(0, 'g');
	buffer.Insert(1, (uint8_t *)"oodbye", 6);

	buffer.Reclaim();
	buffer.Append("and now for something completely different...");

	std::cout << buffer.Contents() << std::endl;
    std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;
    buffer.Resize(128);
    std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;
    buffer.Trim();
    std::cout << "Length: " << buffer.Length() << ", capacity " << buffer.Capacity() << std::endl;


    return 0;
}
