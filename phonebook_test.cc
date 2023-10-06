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
/*
constexpr char		TEST_PBSTR4[] = "spam";
constexpr uint8_t	TEST_PBSTRLEN4 = 4;
constexpr char		TEST_PBSTR5[] = "xyzzx";
constexpr uint8_t	TEST_PBSTRLEN5 = 5;
constexpr char		TEST_PBSTR6[] = "corvid";
constexpr uint8_t	TEST_PBSTRLEN6 = 6;
 */


static bool
test_setpb(Phonebook &pb, const char *k, uint8_t kl, const char *v,
	   uint8_t vl)
{
	bool	ok;
	std::cout << "test set " << k << "->" << v << std::endl;
	ok = pb.set(k, kl, v, vl) == 0;
	std::cout << "\tset complete\n";
	return ok;
}


int
main(int argc, const char *argv[])
{
	Arena		arena;
	//TLV::Record	value;
	//TLV::Record	expect;

	std::cout << "TESTPROG: " << argv[0] << std::endl; 
	if (create_arena(arena, ARENA_FILE, ARENA_SIZE, 0644) == -1) {
		abort();
	}

	Phonebook	pb(arena);
	assert(!pb.has(TEST_PBSTR2, TEST_PBSTRLEN2));

	assert(test_setpb(pb, TEST_PBSTR1, TEST_PBSTRLEN1, TEST_PBSTR3,
		      TEST_PBSTRLEN3));
	assert(test_setpb(pb, TEST_PBSTR2, TEST_PBSTRLEN2, TEST_PBSTR3,
		      TEST_PBSTRLEN3));
	
	std::cout << "OK" <<std::endl;
}
