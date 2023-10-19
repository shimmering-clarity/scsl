//
// Project: scccl
// File: test/math/simple_suite_example.cpp
// Author: Kyle Isom
// Date: 2017-06-05
//
// simple_suite_example demonstrates the usage of the SimpleSuite test class
// and serves to unit test the unit tester (qui custodiet ipsos custodes)?
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

#include <iostream>

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
main()
{
	sctest::SimpleSuite TestSuite;
	TestSuite.Setup(prepareTests);
	TestSuite.Teardown(destroyTests);
	TestSuite.AddTest("1 + 1", addOne);
	TestSuite.AddTest("fourness", four);
	TestSuite.AddFailingTest("self-evident truth", nope);

	bool	result = TestSuite.Run();
	if (TestSuite.IsReportReady()) {
		auto report = TestSuite.GetReport();
		std::cout << report.Failing << " / " << report.Total;
		std::cout << " tests failed.\n";
	}

	if (result) {
		return 0;
	}
	else {
		return 1;
	}
}
