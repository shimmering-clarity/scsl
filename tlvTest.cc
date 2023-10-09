#include <cassert>
#include <cstring>
#include <iostream>

#include "Arena.h"
#include "TLV.h"

#include "testFixtures.h"


static uint8_t		arenaBuffer[ARENA_SIZE];


bool
tlvTestSuite(Arena &backend)
{
	TLV::Record	 rec1, rec2, rec3, rec4;
	uint8_t		*cursor = NULL;

	TLV::SetRecord(rec1, 1, TEST_STRLEN1, TEST_STR1);
	TLV::SetRecord(rec2, 2, TEST_STRLEN2, TEST_STR2);
	TLV::SetRecord(rec3, 1, TEST_STRLEN4, TEST_STR4);
	rec4.Tag = 1;

	assert(TLV::WriteToMemory(backend, cursor, rec1) != NULL);
	assert((cursor = TLV::WriteToMemory(backend, cursor, rec2)) != NULL);
	assert(TLV::WriteToMemory(backend, cursor, rec3) != NULL);
	cursor = NULL;

	// the cursor should point at the next record,
	// and rec4 should contain the same data as rec1.
	cursor = TLV::FindTag(backend, cursor, rec4);
	assert(cursor != NULL); 
	assert(cursor != backend.Store);
	assert(cmpRecord(rec1, rec4));

	cursor = TLV::FindTag(backend, cursor, rec4);
	assert(cursor != NULL);
	assert(cmpRecord(rec3, rec4));

	TLV::SetRecord(rec4, 3, TEST_STRLEN3, TEST_STR3);
	assert(TLV::WriteToMemory(backend, NULL, rec4));

	rec4.Tag = 2;
	cursor = TLV::FindTag(backend, NULL, rec4);
	assert(cursor != NULL);

	TLV::DeleteRecord(backend, cursor);
	assert(cursor[0] == 3);

	return true;
}

bool
runSuite(Arena &backend, const char *label)
{
	DisplayArena(backend);

	std::cout << "running test suite " << label << ": ";
	if (!tlvTestSuite(backend)) {
		std::cout << "FAILED" << std::endl;
		return false;
	}
	std::cout << "OK" << std::endl;

	std::cout << "\tdestroying arena: ";
	if (DestroyArena(backend) != 0) {
		std::cout << "FAILED" << std::endl;
		return false;
	}

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
	InitializeArena(arenaStatic);
	InitializeArena(arenaMem);

	if (-1 == NewStaticArena(arenaStatic, arenaBuffer, ARENA_SIZE)) {
		abort();
	} else if (!runSuite(arenaStatic, "arenaStatic")) {
		abort();
	}
	ClearArena(arenaStatic);

	#if defined(__linux__)
	Arena	arenaFile;
	InitializeArena(arenaFile);

	if (-1 == CreateArena(arenaFile, ARENA_FILE, ARENA_SIZE, 0644)) {
		abort();
	} else if (!runSuite(arenaFile, "arenaFile")) {
		abort();
	}
	#endif

	if (-1 == AllocNewArena(arenaMem, ARENA_SIZE)) {
		abort();
	} else if (!runSuite(arenaMem, "arenaMem")) {
		abort();
	}

	std::cout << "OK" << std::endl;
	return 0;
}
