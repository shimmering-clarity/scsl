#include <cassert>
#include <iostream>

#include "Arena.h"
#include "Dictionary.h"
#include "testFixtures.h"


constexpr char		TEST_KVSTR1[] = "foo";
constexpr uint8_t	TEST_KVSTRLEN1 = 3;
constexpr char		TEST_KVSTR2[] = "baz";
constexpr uint8_t	TEST_KVSTRLEN2 = 3;
constexpr char		TEST_KVSTR3[] = "quux";
constexpr uint8_t	TEST_KVSTRLEN3 = 4;
constexpr char		TEST_KVSTR4[] = "spam";
constexpr uint8_t	TEST_KVSTRLEN4 = 4;
constexpr char		TEST_KVSTR5[] = "xyzzx";
constexpr uint8_t	TEST_KVSTRLEN5 = 5;
constexpr char		TEST_KVSTR6[] = "corvid";
constexpr uint8_t	TEST_KVSTRLEN6 = 6;


static bool
testSetKV(Dictionary &pb, const char *k, uint8_t kl, const char *v,
	  uint8_t vl)
{
	bool	ok;
	std::cout << "test Set " << k << "->" << v << std::endl;
	ok = pb.Set(k, kl, v, vl) == 0;
	std::cout << "\tSet complete\n";
	return ok;
}


int
main(int argc, const char *argv[])
{
	(void)argc; (void)argv;

	Arena		arena;
	TLV::Record	value;
	TLV::Record	expect;

	std::cout << "TESTPROG: " << argv[0] << std::endl;

	#if defined(__linux__)
	if (arena.Create(ARENA_FILE, ARENA_SIZE, 0644) == -1) {
		abort();
	}
	#else
	if (AllocNewArena(arena, ARENA_SIZE) == -1) {
		abort();
	}
	#endif
	std::cout << arena << std::endl;
	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN3, TEST_KVSTR3);

	Dictionary	dict(arena);
	assert(!dict.Has(TEST_KVSTR2, TEST_KVSTRLEN2));

	assert(testSetKV(dict, TEST_KVSTR1, TEST_KVSTRLEN1, TEST_KVSTR3,
			 TEST_KVSTRLEN3));
	dict.DumpKVPairs();
	assert(testSetKV(dict, TEST_KVSTR2, TEST_KVSTRLEN2, TEST_KVSTR3,
			 TEST_KVSTRLEN3));
	dict.DumpKVPairs();
	assert(dict.Has(TEST_KVSTR2, TEST_KVSTRLEN2));
	assert(testSetKV(dict, TEST_KVSTR4, TEST_KVSTRLEN4, TEST_KVSTR5,
			 TEST_KVSTRLEN5));
	dict.DumpKVPairs();
	assert(dict.Lookup(TEST_KVSTR2, TEST_KVSTRLEN2, value));

	assert(cmpRecord(value, expect));

	std::cout << "test overwriting key" << std::endl;
	assert(testSetKV(dict, TEST_KVSTR2, TEST_KVSTRLEN2, TEST_KVSTR6,
			 TEST_KVSTRLEN6));
	dict.DumpKVPairs();
	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN6, TEST_KVSTR6);
	std::cout << "\tlookup" << std::endl;
	assert(dict.Lookup(TEST_KVSTR2, TEST_KVSTRLEN2, value));
	std::cout << "\tcompare records" << std::endl;
	assert(cmpRecord(value, expect));

	std::cout << "\tadd new key to dictionary" << std::endl;
	assert(testSetKV(dict, TEST_KVSTR3, TEST_KVSTRLEN3, TEST_KVSTR5,
			 TEST_KVSTRLEN5));
	dict.DumpKVPairs();

	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN5, TEST_KVSTR5);
	assert(dict.Lookup(TEST_KVSTR4, TEST_KVSTRLEN4, value));
	assert(cmpRecord(value, expect));

	std::cout << "OK" <<std::endl;

	// Dump the generated arena for inspection later.
	#if defined(__linux__)
	#else
	dict.DumpToFile(ARENA_FILE);
	#endif

	arena.Clear();
	dict.DumpKVPairs();
}
