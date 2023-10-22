///
/// \file test/dictionary.cc
/// \author K.Isom <kyle@imap.cc>
/// \date 2023-10-05
/// \brief Unit tests on the scsl::Dictionary class.
///
/// \section COPYRIGHT
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that
/// the above copyright notice and this permission notice appear in all /// copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
/// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
/// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
/// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
/// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
/// PERFORMANCE OF THIS SOFTWARE.
///

#include <iostream>

#include <scsl/Arena.h>
#include <scsl/Dictionary.h>
#include <scsl/Flags.h>
#include <sctest/SimpleSuite.h>
#include <sctest/Checks.h>

#include "test_fixtures.h"


using namespace scsl;
using namespace sctest;


constexpr char    TEST_KVSTR1[]  = "foo";
constexpr uint8_t TEST_KVSTRLEN1 = 3;
constexpr char    TEST_KVSTR2[]  = "baz";
constexpr uint8_t TEST_KVSTRLEN2 = 3;
constexpr char    TEST_KVSTR3[]  = "quux";
constexpr uint8_t TEST_KVSTRLEN3 = 4;
constexpr char    TEST_KVSTR4[]  = "spam";
constexpr uint8_t TEST_KVSTRLEN4 = 4;
constexpr char    TEST_KVSTR5[]  = "xyzzx";
constexpr uint8_t TEST_KVSTRLEN5 = 5;
constexpr char    TEST_KVSTR6[]  = "corvid";
constexpr uint8_t TEST_KVSTRLEN6 = 6;


static bool
testSetKV(Dictionary &pb, const char *k, uint8_t kl, const char *v,
	  uint8_t vl)
{
	return pb.Set(k, kl, v, vl) == 0;
}

bool
dictionaryTest()
{
	Arena       arena;
	TLV::Record value;
	TLV::Record expect;

	if (arena.Create(ARENA_FILE, ARENA_SIZE) == -1) {
		abort();
	}

	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN3, TEST_KVSTR3);

	Dictionary dict(arena);
	SCTEST_CHECK_FALSE(dict.Contains(TEST_KVSTR2, TEST_KVSTRLEN2));
	SCTEST_CHECK(testSetKV(dict, TEST_KVSTR1, TEST_KVSTRLEN1, TEST_KVSTR3,
			       TEST_KVSTRLEN3));

	SCTEST_CHECK(testSetKV(dict, TEST_KVSTR2, TEST_KVSTRLEN2, TEST_KVSTR3,
			       TEST_KVSTRLEN3));

	SCTEST_CHECK(dict.Contains(TEST_KVSTR2, TEST_KVSTRLEN2));
	SCTEST_CHECK(testSetKV(dict, TEST_KVSTR4, TEST_KVSTRLEN4, TEST_KVSTR5,
			       TEST_KVSTRLEN5));

	SCTEST_CHECK(dict.Lookup(TEST_KVSTR2, TEST_KVSTRLEN2, value));

	SCTEST_CHECK(cmpRecord(value, expect));


	SCTEST_CHECK(testSetKV(dict, TEST_KVSTR2, TEST_KVSTRLEN2, TEST_KVSTR6,
			       TEST_KVSTRLEN6));

	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN6, TEST_KVSTR6);
	SCTEST_CHECK(dict.Lookup(TEST_KVSTR2, TEST_KVSTRLEN2, value));

	SCTEST_CHECK(cmpRecord(value, expect));

	SCTEST_CHECK(testSetKV(dict, TEST_KVSTR3, TEST_KVSTRLEN3, TEST_KVSTR5,
			       TEST_KVSTRLEN5));

	TLV::SetRecord(expect, DICTIONARY_TAG_VAL, TEST_KVSTRLEN5, TEST_KVSTR5);
	SCTEST_CHECK(dict.Lookup(TEST_KVSTR4, TEST_KVSTRLEN4, value));
	SCTEST_CHECK(cmpRecord(value, expect));

	arena.Clear();
	return true;
}


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet    = false;
	auto flags    = new scsl::Flags("test_dictionary",
					"This test validates the Dictionary class.");
	flags->Register("-n", false, "don't print the report");
	flags->Register("-q", false, "suppress test output");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
	}

	sctest::SimpleSuite suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	suite.AddTest("dictionaryTest", dictionaryTest);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
