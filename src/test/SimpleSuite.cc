///
/// \file SimpleSuite.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Defines a simple unit testing framework.
///
/// Copyright 2017 K. Isom <kyle@imap.cc>
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

#include <chrono>
#include <iostream>

#include <sctest/SimpleSuite.h>


namespace sctest {

#define unless(cond) if (!(cond))


static bool
stub()
{ return true; }

SimpleSuite::SimpleSuite()
    : quiet(false), fnSetup(stub), fnTeardown(stub), tests(),
      report(), hasRun(false)
{

}


void
SimpleSuite::AddTest(std::string name, std::function<bool()> test)
{
	TestCase test_case = {name, test};
	tests.push_back(test_case);
}


void
SimpleSuite::AddFailingTest(std::string name, std::function<bool()> test)
{
	// auto ntest = [&test]() { return !test(); };
	TestCase test_case = {name, [&test]() { return !test(); }};
	tests.push_back(test_case);
}


bool
SimpleSuite::Run()
{
	report.Start = std::chrono::steady_clock::now();
	unless(quiet) { std::cout << "Setting up the tests.\n"; }
	unless(fnSetup()) { return false; }

	// Reset the failed test counts.
	report.Failing = 0;

	bool result = true;
	hasRun = true;
	report.Total = tests.size();
	for (size_t i = 0; i < report.Total && result; i++) {
		TestCase tc = tests.at(i);
		unless(quiet) {
			std::cout << "[" << i + 1 << "/" << report.Total << "] Running test " << tc.name << ": ";
		}

		result = tc.test();
		if (quiet) { continue; }

		if (result) {
			std::cout << "[PASS]";
		} else {
			std::cout << "[FAIL]";
			report.Failing++;
		}
		std::cout << "\n";
	}

	unless(quiet) { std::cout << "Tearing down the tests.\n"; }
	unless(fnTeardown()) { return false; }
	report.End = std::chrono::steady_clock::now();
	return result;
}


Report
SimpleSuite::GetReport()
{
	return report;
}


} // end namespace sctest