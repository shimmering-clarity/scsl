#include <cassert>
#include <iostream>

#include "Arena.h"
#include "Phonebook.h"
#include "test_fixtures.h"


constexpr char		TEST_PBSTR1[] = "foo";
constexpr uint8_t	TEST_PBSTRLEN1 = 3;
constexpr char		TEST_PBSTR2[] = "baz";
constexpr uint8_t	TEST_PBSTRLEN2 = 3;
constexpr char		TEST_PBSTR3[] = "quux";
constexpr uint8_t	TEST_PBSTRLEN3 = 4;
constexpr char		TEST_PBSTR4[] = "spam";
constexpr uint8_t	TEST_PBSTRLEN4 = 4;
constexpr char		TEST_PBSTR5[] = "xyzzx";
constexpr uint8_t	TEST_PBSTRLEN5 = 5;
constexpr char		TEST_PBSTR6[] = "corvid";
constexpr uint8_t	TEST_PBSTRLEN6 = 6;


static bool
test_setpb(Phonebook &pb, const char *k, uint8_t kl, const char *v,
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
	Arena		arena;
	TLV::Record	value;
	TLV::Record	expect;

	std::cout << "TESTPROG: " << argv[0] << std::endl;
	InitializeArena(arena);

	#if defined(__linux__)
	if (CreateArena(arena, ARENA_FILE, ARENA_SIZE, 0644) == -1) {
		abort();
	}
	#else
	if (AllocNewArena(arena, ARENA_SIZE) == -1) {
		abort();
	}
	#endif
	DisplayArena(arena);

	TLV::SetRecord(expect, PHONEBOOK_VAL_TAG, TEST_PBSTRLEN3, TEST_PBSTR3);

	Phonebook	pb(arena);
	assert(!pb.Has(TEST_PBSTR2, TEST_PBSTRLEN2));

	assert(test_setpb(pb, TEST_PBSTR1, TEST_PBSTRLEN1, TEST_PBSTR3,
		      TEST_PBSTRLEN3));
	pb.DumpKVPairs();
	assert(test_setpb(pb, TEST_PBSTR2, TEST_PBSTRLEN2, TEST_PBSTR3,
		      TEST_PBSTRLEN3));
	pb.DumpKVPairs();
	assert(pb.Has(TEST_PBSTR2, TEST_PBSTRLEN2));
	assert(test_setpb(pb, TEST_PBSTR4, TEST_PBSTRLEN4, TEST_PBSTR5,
			  TEST_PBSTRLEN5));
	pb.DumpKVPairs();
	assert(pb.Lookup(TEST_PBSTR2, TEST_PBSTRLEN2, value));

	assert(cmp_record(value, expect));

	std::cout << "test overwriting key" << std::endl;
	assert(test_setpb(pb, TEST_PBSTR2, TEST_PBSTRLEN2, TEST_PBSTR6,
			  TEST_PBSTRLEN6));
	pb.DumpKVPairs();
	TLV::SetRecord(expect, PHONEBOOK_VAL_TAG, TEST_PBSTRLEN6, TEST_PBSTR6);
	std::cout << "\tlookup" << std::endl;
	assert(pb.Lookup(TEST_PBSTR2, TEST_PBSTRLEN2, value));
	std::cout << "\tcompare records" << std::endl;
	assert(cmp_record(value, expect));

	std::cout << "\tadd new key to phonebook" << std::endl;
	assert(test_setpb(pb, TEST_PBSTR3, TEST_PBSTRLEN3, TEST_PBSTR5,
			  TEST_PBSTRLEN5));
	pb.DumpKVPairs();

	TLV::SetRecord(expect, PHONEBOOK_VAL_TAG, TEST_PBSTRLEN5, TEST_PBSTR5);
	assert(pb.Lookup(TEST_PBSTR4, TEST_PBSTRLEN4, value));
	assert(cmp_record(value, expect));

	std::cout << "OK" <<std::endl;

	// Dump the generated arena for inspection later.
	#if defined(__linux__)
	#else
	pb.DumpToFile(ARENA_FILE);
	#endif

	ClearArena(arena);
	pb.DumpKVPairs();
}
