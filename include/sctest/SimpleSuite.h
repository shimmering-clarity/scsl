//
// Project: scccl
// File: include/test/SimpleSuite.h
// Author: Kyle Isom
// Date: 2017-06-05
// Namespace: test
//
// SimpleSuite.h defines the SimpleSuite class for unit testing.
//
// Copyright 2017 Kyle Isom <kyle@imap.cc>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef __SCTEST_SIMPLESUITE_H
#define __SCTEST_SIMPLESUITE_H

// SimpleSuite.h
// This header file defines the interface for a simple suite of tests.

#include <functional>
#include <string>
#include <vector>

#include <sctest/Report.h>

namespace sctest {

typedef struct {
	std::string			name;
	std::function<bool(void)>	test;
} TestCase;

class SimpleSuite {
	public:
	SimpleSuite();

	// Silence suppresses output.
	void Silence(void) { quiet = true; }

	// Setup defines a setup function; this should be a predicate. This function
	// is called at the start of the Run method, before tests are run.
	void Setup(std::function<bool(void)> setupFn) { fnSetup = setupFn; }

	// Teardown defines a teardown function; this should be a predicate. This
	// function is called at the end of the Run method, after all tests have run.
	void Teardown(std::function<bool(void)> teardownFn) { fnTeardown = teardownFn; }

	// AddTest is used to add a test that is expected to return true.
	void AddTest(std::string, std::function<bool(void)>);

	// AddFailingTest is used to add a test that is expected to return false.
	void AddFailingTest(std::string, std::function<bool(void)>);

	bool Run(void);

	// Reporting methods.

	// Reset clears the report statistics.
	void Reset(void) { report.Failing = report.Total = 0; hasRun = false; };

	// IsReportReady returns true if a report is ready.
	bool IsReportReady(void) { return hasRun; }

	// Report returns a Report.
	Report GetReport(void);
	
	private:
	bool				quiet;
	std::function<bool(void)>	fnSetup, fnTeardown;
	std::vector<TestCase>		tests;

	// Report functions.
	Report				report;
	bool				hasRun; // Have the tests been run yet?
};

} // end namespace test
#endif
