///
/// \file test/simple_suite_example.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
///
/// simple_suite_example demonstrates the usage of the SimpleSuite test class
/// and serves to unit test the unit tester (qui custodiet ipsos custodes)?
///
///
/// Copyright 2017 K. Isom <kyle@imap.cc>
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

#include <iostream>

#include <scsl/Flags.h>
#include <sctest/SimpleSuite.h>


static bool
prepareTests() 
{ 
	std::cout << "time passes...\n";
	std::cout << "tests are ready.\n";
	return true;
}


static bool
destroyTests() 
{ 
	std::cout << "time passes...\n" ;
	std::cout << "tests have been destroyed.\n";
	return true;
}


static bool addOne() { return 1 + 1 == 2; }
static bool four()   { return 2 + 2 == 4; }
static bool nope()   { return 2 + 2 == 5; }


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet = false;
	auto flags = new scsl::Flags("test_orientation",
				     "This test validates various orientation-related components in scmp.");
	flags->Register("-n", false, "don't print the report");
	flags->Register("-q", false, "suppress test output");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
		exit(1);
	}

	sctest::SimpleSuite suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	suite.Setup(prepareTests);
	suite.Teardown(destroyTests);
	suite.AddTest("1 + 1", addOne);
	suite.AddTest("fourness", four);
	suite.AddFailingTest("self-evident truth", nope);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
