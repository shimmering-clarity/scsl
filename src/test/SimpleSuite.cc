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
	this->Reset();
}


void
SimpleSuite::Silence()
{
	// Silence will fall.
	quiet = true;
}


void
SimpleSuite::AddTest(std::string name, std::function<bool()> test)
{
	const UnitTest test_case = {std::move(name), std::move(test), true};
	tests.push_back(test_case);
}


void
SimpleSuite::AddFailingTest(std::string name, std::function<bool()> test)
{
	const UnitTest test_case = {std::move(name), test, false};
	tests.push_back(test_case);
}


bool
SimpleSuite::Run()
{
	report.Reset(this->tests.size());

	unless(quiet) { std::cout << "Setting up the tests.\n"; }
	unless(fnSetup()) { return false; }

	this->hasRun = true;
	this->hasPassed = true;

	for (size_t i = 0; i < this->report.Total() && this->hasPassed; i++) {
		const UnitTest testCase = this->tests.at(i);
		unless(quiet) {
			std::cout << "[" << i + 1 << "/"
				  << this -> report.Total()
				  << "] Running test "
				  << testCase.name << ": ";
		}

		this->hasPassed = (testCase.test() == testCase.expect);
		if (this->hasPassed) {
			report.Passed();
		} else {
			report.Failed();
		}

		if (quiet) { continue; }

		if (this->hasPassed) {
			std::cout << "[PASS]";
		} else {
			std::cout << "[FAIL]";
		}
		std::cout << "\n";
	}

	unless(quiet) { std::cout << "Tearing down the tests.\n"; }
	unless(fnTeardown()) { return false; }
	report.End();
	return this->hasPassed;
}


void
SimpleSuite::Reset()
{
	this->report.Reset(0);
	this->hasRun = false;
	this->hasPassed = false;
}


bool
SimpleSuite::HasRun() const
{
	return this->hasRun;
}


Report
SimpleSuite::GetReport()
{
	return report;
}


std::ostream &
operator<<(std::ostream &os, SimpleSuite &suite)
{
	if (suite.HasRun()) {
		os << "OK: " << suite.GetReport();
	} else {
		os << "Test suite hasn't run.";
	}

	return os;
}


} // end namespace sctest
