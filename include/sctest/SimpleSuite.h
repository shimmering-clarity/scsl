///
/// \file SimpleSuite.h
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

#ifndef SCTEST_SIMPLESUITE_H
#define SCTEST_SIMPLESUITE_H


#include <functional>
#include <string>
#include <vector>

#include <sctest/Report.h>

namespace sctest {


/// \brief UnitTest describes a single unit test. It is a predicate:
///        did the test pass?
struct UnitTest {
	/// What name should be shown when running tests?
	std::string		name;

	/// This is the test function to be run.
	std::function<bool()>	test;
};

/// \brief SimpleSuite is a test-running harness for simple tests.
///
/// A simple test is defined as a test that takes no arguments and
/// returns a boolean status where true indicates the test has passed.
class SimpleSuite {
public:
	SimpleSuite();

	/// \brief Silence suppresses output.
	void Silence();

	/// \brief Define a suite setup function.
	///
	/// If present, this setup function is called at the start of
	/// the Run method, before tests are run. It should be a
	/// predicate: if it returns false, tests automatically fail.
	void Setup(std::function<bool(void)> setupFn) { fnSetup = setupFn; }

	/// \brief Define a teardown function.
	///
	/// If present, this teardown function is called at the end of
	/// the Run method, after all tests have run.
	void Teardown(std::function<bool(void)> teardownFn) { fnTeardown = teardownFn; }

	/// \brief Register a new simple test.
	///
	/// \param label The text that will identify test when
	///        running.
	/// \param test This test should return true if the test has
	/// 	   passed.
	void AddTest(std::string label, std::function<bool(void)> test);

	/// \brief Register a test that is expected to return false.
	///
	/// \param label The text that will identify test when
	///        running.
	/// \param test This test should return false if the test has
	/// 	   passed.
	void AddFailingTest(std::string label, std::function<bool(void)> test);

	/// \brief Run all the registered tests.
	///
	/// \return True if all tests have passed.
	bool Run();

	/// Reporting methods.

	/// \brief Reset clears the report statistics.
	///
	/// Reset will preserve the setup and teardown functions, just
	/// resetting the suite's internal state.
	void Reset();

	/// \brief
	// HasRun returns true if a report is ready.
	bool HasRun() const;

	// Report returns a Report.
	Report GetReport(void);

private:
	bool				quiet;
	std::function<bool(void)> fnSetup, fnTeardown;
	std::vector<UnitTest>     tests;

	// Report functions.
	Report	report;
	bool	hasRun; // Have the tests been run yet?
	bool	hasPassed;
};


std::ostream& operator<<(std::ostream& os, SimpleSuite &suite);


} // namespace sctest
#endif
