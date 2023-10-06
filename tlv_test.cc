#include <cassert>
#include <cstring>
#include <iostream>

#include "Arena.h"
#include "TLV.h"

#include "test_fixtures.h"


static uint8_t		arena_buffer[ARENA_SIZE];


static bool
cmp_record(TLV::Record &a, TLV::Record &b)
{
	if (a.Tag != b .Tag) {
		return false;
	}

	if (a.Len != b.Len) {
		return false;
	}

	if (memcmp(a.Val, b.Val, a.Len) != 0) {
		return false;
	}

	return true;
}


bool
tlv_test_suite(Arena &backend)
{
	TLV::Record	 rec1, rec2, rec3, rec4;
	uint8_t		*cursor = NULL;

	TLV::set_record(rec1, 1, TEST_STRLEN1, TEST_STR1);
	TLV::set_record(rec2, 2, TEST_STRLEN2, TEST_STR2);
	TLV::set_record(rec3, 1, TEST_STRLEN4, TEST_STR4);
	rec4.Tag = 1;

	assert(TLV::write_to_memory(backend, cursor, rec1) != NULL);
	assert((cursor = TLV::write_to_memory(backend, cursor, rec2)) != NULL);
	assert(TLV::write_to_memory(backend, cursor, rec3) != NULL);
	cursor = NULL;

	// the cursor should point at the next record,
	// and rec4 should contain the same data as rec1.
	cursor = TLV::find_tag(backend, cursor, rec4);
	assert(cursor != NULL); 
	assert(cursor != backend.store);
	assert(cmp_record(rec1, rec4));

	cursor = TLV::find_tag(backend, cursor, rec4);
	assert(cursor != NULL);
	assert(cmp_record(rec3, rec4));

	TLV::set_record(rec4, 3, TEST_STRLEN3, TEST_STR3);
	assert(TLV::write_to_memory(backend, NULL, rec4));

	rec4.Tag = 2;
	cursor = TLV::find_tag(backend, NULL, rec4);
	assert(cursor != NULL);

	TLV::delete_record(backend, cursor);
	assert(cursor[0] == 3);

	return true;
}

bool
run_suite(Arena &backend, const char *label)
{
	std::cout << "running test suite " << label << ": ";
	if (!tlv_test_suite(backend)) {
		std::cout << "FAILED" << std::endl;
		return false;
	}
	std::cout << "OK" << std::endl;

	std::cout << "\tdestroying arena: ";
	if (arena_destroy(backend) != 0) {
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

	if (-1 == new_arena(arena_static, arena_buffer, ARENA_SIZE)) {
		abort();
	} else if (!run_suite(arena_static, "arena_static")) {
		abort();
	}

	if (-1 == create_arena(arena_file, ARENA_FILE, ARENA_SIZE, 0644)) {
		abort();
	} else if (!run_suite(arena_file, "arena_file")) {
		abort();
	}

	if (-1 == alloc_new_arena(arena_mem, ARENA_SIZE)) {
		abort();
	} else if (!run_suite(arena_mem, "arena_mem")) {
		abort();
	}


	std::cout << "OK" << std::endl;
	return 0;
}
