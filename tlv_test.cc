#include <cassert>
#include <cstring>
#include <iostream>

#include "Arena.h"
#include "TLV.h"

#include "test_fixtures.h"


static uint8_t		arena_buffer[ARENA_SIZE];


bool
tlv_test_suite(Arena &backend)
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
	assert(cmp_record(rec1, rec4));

	cursor = TLV::FindTag(backend, cursor, rec4);
	assert(cursor != NULL);
	assert(cmp_record(rec3, rec4));

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
run_suite(Arena &backend, const char *label)
{
	DisplayArena(backend);

	std::cout << "running test suite " << label << ": ";
	if (!tlv_test_suite(backend)) {
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
	Arena	arena_static;
	Arena	arena_mem;
	Arena	arena_file;

	std::cout << "TESTPROG: " << argv[0] << std::endl;
	InitializeArena(arena_static);
	InitializeArena(arena_mem);
	InitializeArena(arena_file);

	if (-1 == NewStaticArena(arena_static, arena_buffer, ARENA_SIZE)) {
		abort();
	} else if (!run_suite(arena_static, "arena_static")) {
		abort();
	}
	ClearArena(arena_static);

	#if defined(__linux__)
	if (-1 == CreateArena(arena_file, ARENA_FILE, ARENA_SIZE, 0644)) {
		abort();
	} else if (!run_suite(arena_file, "arena_file")) {
		abort();
	}
	#endif

	if (-1 == AllocNewArena(arena_mem, ARENA_SIZE)) {
		abort();
	} else if (!run_suite(arena_mem, "arena_mem")) {
		abort();
	}

	std::cout << "OK" << std::endl;
	return 0;
}
