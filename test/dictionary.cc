#include <iostream>

#include <scsl/Arena.h>
#include <scsl/Dictionary.h>
#include <sctest/Assert.h>
#include "test_fixtures.h"


using namespace scsl;


constexpr char TEST_KVSTR1[] = "foo";
constexpr uint8_t TEST_KVSTRLEN1 = 3;
constexpr char TEST_KVSTR2[] = "baz";
constexpr uint8_t TEST_KVSTRLEN2 = 3;
constexpr char TEST_KVSTR3[] = "quux";
constexpr uint8_t TEST_KVSTRLEN3 = 4;
constexpr char TEST_KVSTR4[] = "spam";
constexpr uint8_t TEST_KVSTRLEN4 = 4;
constexpr char TEST_KVSTR5[] = "xyzzx";
constexpr uint8_t TEST_KVSTRLEN5 = 5;
constexpr char TEST_KVSTR6[] = "corvid";
constexpr uint8_t TEST_KVSTRLEN6 = 6;


static bool
testSetKV(Dictionary &pb, const char *k, uint8_t kl, const char *v,
	  uint8_t vl)
{
	bool ok;
	std::cout << "test Set " << k << "->" << v << "\n";
	ok = pb.Set(k, kl, v, vl) == 0;
	std::cout << "\tSet complete\n";
	return ok;
}


int
main(int argc, const char *argv[])
{
	(void) argc;
	(void) argv;

	Arena arena;
	TLV::Record value;
	TLV::Record expect;

	std::cout << "TESTPROG: " << argv[0] << "\n";

#if defined(__linux__)
	if (arena.Create(ARENA_FILE, ARENA_SIZE) == -1) {
		abort();
	}
#else
	if (arena.SetAlloc(ARENA_SIZE) == -1) {
		abort();
	}
#endif
	std::cout << arena << "\n";
	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN3, TEST_KVSTR3);

	Dictionary dict(arena);
	TestAssert(!dict.Contains(TEST_KVSTR2, TEST_KVSTRLEN2));

	TestAssert(testSetKV(dict, TEST_KVSTR1, TEST_KVSTRLEN1, TEST_KVSTR3,
			     TEST_KVSTRLEN3));
	std::cout << dict;
	TestAssert(testSetKV(dict, TEST_KVSTR2, TEST_KVSTRLEN2, TEST_KVSTR3,
			     TEST_KVSTRLEN3));
	std::cout << dict;
	TestAssert(dict.Contains(TEST_KVSTR2, TEST_KVSTRLEN2));
	TestAssert(testSetKV(dict, TEST_KVSTR4, TEST_KVSTRLEN4, TEST_KVSTR5,
			     TEST_KVSTRLEN5));
	std::cout << dict;
	TestAssert(dict.Lookup(TEST_KVSTR2, TEST_KVSTRLEN2, value));

	TestAssert(cmpRecord(value, expect));

	std::cout << "test overwriting key" << "\n";
	TestAssert(testSetKV(dict, TEST_KVSTR2, TEST_KVSTRLEN2, TEST_KVSTR6,
			     TEST_KVSTRLEN6));
	std::cout << dict;
	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN6, TEST_KVSTR6);
	std::cout << "\tlookup" << "\n";
	TestAssert(dict.Lookup(TEST_KVSTR2, TEST_KVSTRLEN2, value));
	std::cout << "\tcompare records" << "\n";
	TestAssert(cmpRecord(value, expect));

	std::cout << "\tadd new key to dictionary" << "\n";
	TestAssert(testSetKV(dict, TEST_KVSTR3, TEST_KVSTRLEN3, TEST_KVSTR5,
			     TEST_KVSTRLEN5));
	std::cout << dict;

	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN5, TEST_KVSTR5);
	TestAssert(dict.Lookup(TEST_KVSTR4, TEST_KVSTRLEN4, value));
	TestAssert(cmpRecord(value, expect));

	std::cout << "OK" << "\n";

	// Dump the generated arena for inspection later.
#if defined(__linux__)
#else
	dict.DumpToFile(ARENA_FILE);
#endif

	arena.Clear();
	std::cout << dict;
}
