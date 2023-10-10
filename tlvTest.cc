#include <cassert>
#include <cstring>
#include <exception>
#include <iostream>

#include "Arena.h"
#include "Test.h"
#include "TLV.h"

#include "testFixtures.h"

using namespace klib;


static uint8_t		arenaBuffer[ARENA_SIZE];


void
tlvTestSuite(Arena &backend)
{
	TLV::Record	 rec1, rec2, rec3, rec4;
	uint8_t		*cursor = nullptr;

	std::cout << "\tSetting first three records." << std::endl;
	TLV::SetRecord(rec1, 1, TEST_STRLEN1, TEST_STR1);
	TLV::SetRecord(rec2, 2, TEST_STRLEN2, TEST_STR2);
	TLV::SetRecord(rec3, 1, TEST_STRLEN4, TEST_STR4);
	rec4.Tag = 1;

	std::cout << "\twriting new rec1" << std::endl;
	assert(TLV::WriteToMemory(backend, cursor, rec1) != nullptr);
	std::cout << "\twriting new rec2" << std::endl;
	assert((cursor = TLV::WriteToMemory(backend, cursor, rec2)) != nullptr);
	std::cout << "\twriting new rec3" << std::endl;
	assert(TLV::WriteToMemory(backend, cursor, rec3) != nullptr);
	cursor = nullptr;

	// the cursor should point at the next record,
	// and rec4 should contain the same data as rec1.
	std::cout << "\tFindTag 1" << std::endl;
	cursor = TLV::FindTag(backend, cursor, rec4);
	assert(cursor != nullptr); 
	assert(cursor != backend.NewCursor());
	assert(cmpRecord(rec1, rec4));

	std::cout << "\tFindTag 2" << std::endl;
	cursor = TLV::FindTag(backend, cursor, rec4);
	assert(cursor != nullptr);
	assert(cmpRecord(rec3, rec4));

	TLV::SetRecord(rec4, 3, TEST_STRLEN3, TEST_STR3);
	assert(TLV::WriteToMemory(backend, nullptr, rec4));

	rec4.Tag = 2;
	cursor = TLV::FindTag(backend, nullptr, rec4);
	assert(cursor != nullptr);

	TLV::DeleteRecord(backend, cursor);
	assert(cursor[0] == 3);
}

bool
runSuite(Arena &backend, const char *label)
{
	std::cout << backend << std::endl;
	std::cout << "running test suite " << label << ": ";
	try {
		tlvTestSuite(backend);
	} catch (std::exception &exc){
		std::cout << "FAILED" << std::endl;
		return false;
	}
	std::cout << "OK" << std::endl;

	std::cout << "\tdestroying arena: ";
	backend.Destroy();

	std::cout << "OK" << std::endl;
	return true;
}


int
main(int argc, const char *argv[])
{
	(void)argc; (void)argv;

	Arena	arenaStatic;
	Arena	arenaMem;

	std::cout << "TESTPROG: " << argv[0] << std::endl;

	if (-1 == arenaStatic.SetStatic(arenaBuffer, ARENA_SIZE)) {
		abort();
	} else if (!runSuite(arenaStatic, "arenaStatic")) {
		abort();
	}
	arenaStatic.Clear();

	Arena	arenaFile;

	if (-1 == arenaFile.Create(ARENA_FILE, ARENA_SIZE)) {
		abort();
	} else if (!runSuite(arenaFile, "arenaFile")) {
		abort();
	}

	if (-1 == arenaMem.SetAlloc(ARENA_SIZE)) {
		abort();
	} else if (!runSuite(arenaMem, "arenaMem")) {
		abort();
	}
	arenaMem.Clear();

	std::cout << "OK" << std::endl;
	return 0;
}
