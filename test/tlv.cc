///
/// \file test/tlv.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-05
/// \brief Unit tests for the TLV namespace.
///
/// simple_suite_example demonstrates the usage of the SimpleSuite test class
/// and serves to unit test the unit tester (qui custodiet ipsos custodes)?
///
/// \section COPYRIGHT
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that the
/// above copyright notice and this permission notice appear in all copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
/// BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
/// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
/// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
/// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
/// SOFTWARE.
///

#include <exception>
#include <iostream>

#include <scsl/Arena.h>
#include <scsl/Flags.h>
#include <scsl/TLV.h>
#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>

#include "test_fixtures.h"


using namespace scsl;


static uint8_t		arenaBuffer[ARENA_SIZE];


bool
runTLVTest(Arena &backend)
{
	TLV::Record rec1, rec2, rec3, rec4;
	uint8_t     *cursor = nullptr;

	TLV::SetRecord(rec1, 1, TEST_STRLEN1, TEST_STR1);
	TLV::SetRecord(rec2, 2, TEST_STRLEN2, TEST_STR2);
	TLV::SetRecord(rec3, 1, TEST_STRLEN4, TEST_STR4);
	rec4.Tag = 1;

	cursor = TLV::WriteToMemory(backend, cursor, rec1);
	SCTEST_CHECK_NE(cursor, nullptr);

	cursor = TLV::WriteToMemory(backend, cursor, rec2);
	SCTEST_CHECK_NE(cursor, nullptr);

	cursor = TLV::WriteToMemory(backend, cursor, rec3);
	SCTEST_CHECK_NE(cursor, nullptr);


	// the cursor should point At the next record,
	// and rec4 should contain the same data as rec1.
	cursor = TLV::FindTag(backend, nullptr, rec4);
	SCTEST_CHECK_NE(cursor, nullptr);
	SCTEST_CHECK_NE(cursor, backend.Start());
	SCTEST_CHECK(cmpRecord(rec1, rec4));

	cursor = TLV::FindTag(backend, cursor, rec4);
	SCTEST_CHECK_NE(cursor, nullptr);
	SCTEST_CHECK(cmpRecord(rec3, rec4));

	TLV::SetRecord(rec4, 3, TEST_STRLEN3, TEST_STR3);
	SCTEST_CHECK(TLV::WriteToMemory(backend, nullptr, rec4));

	rec4.Tag = 2;
	cursor = TLV::FindTag(backend, nullptr, rec4);
	SCTEST_CHECK_NE(cursor, nullptr);

	TLV::DeleteRecord(backend, cursor);
	SCTEST_CHECK_EQ(cursor[0], 3);
	rec4.Tag = 3;
	cursor = nullptr;

	return true;
}


bool
tlvTestSuite(ArenaType arenaType)
{
	Arena backend;

	switch (arenaType) {
	case ArenaType::Static:
		if (backend.SetStatic(arenaBuffer, ARENA_SIZE) != 0) {
			std::cerr << "[!] failed to set up statically-allocated arena\n";
			return false;
		}
		break;
	case ArenaType::Alloc:
		if (backend.SetAlloc(ARENA_SIZE) != 0) {
			std::cerr << "[!] failed to set up dynamically-allocated arena\n";
			return false;
		}
		break;
	case ArenaType::MemoryMapped:
		if (backend.Create(ARENA_FILE, ARENA_SIZE)) {
			std::cerr << "[!] failed to set up memory-mapped arena\n";
			return false;
		}
		break;
	default:
		std::cerr << "[!] " << static_cast<int>(arenaType) << " is invalid for this test.\n";
		return false;
	}

	auto result = runTLVTest(backend);
	if (!result) {
		std::cerr << "[!] suite failed with " << backend << "\n";
	}
	backend.Destroy();
	return result;
}

std::function<bool()>
buildTestSuite(ArenaType arenaType)
{
	return [arenaType](){
		return tlvTestSuite(arenaType);
	};
}


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet = false;
	auto flags = new scsl::Flags("test_tlv",
				     "This test validates various TLV-related components in scsl.");
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

	suite.AddTest("ArenaStatic", buildTestSuite(ArenaType::Static));
	suite.AddTest("ArenaAlloc", buildTestSuite(ArenaType::Alloc));
	suite.AddTest("ArenaFile", buildTestSuite(ArenaType::MemoryMapped));

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
