//
// Project: scccl
// File: include/test/Report.h
// Author: Kyle Isom
// Date: 2017-06-05
// Namespace: test
//
// Report.h defines a Report structure that contains information about
// the results of unit tests.
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
#ifndef __SCTEST_REPORT_H
#define __SCTEST_REPORT_H

#include <chrono>

namespace sctest {

typedef struct _Report {
	// Failing stores the number of failing tests; for tests added
	// with AddTest, this is a test that returned false. For tests
	// added with AddFailingTest, this is a test that returned true.
	size_t	Failing;

	// Total is the number of tests registered during the last run.
	size_t	Total;

	std::chrono::time_point<std::chrono::steady_clock>	Start;
	std::chrono::time_point<std::chrono::steady_clock>	End;
	std::chrono::duration<double>						Duration;

	_Report();
} Report;

} // end namespace test
#endif
