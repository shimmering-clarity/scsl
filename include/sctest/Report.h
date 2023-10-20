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
#ifndef SCTEST_REPORT_H
#define SCTEST_REPORT_H

#include <chrono>

namespace sctest {


class Report {
public:
	/// \brief Failing returns the count of failed tests.
	///
	/// \details If a test is run and expected to pass, but fails,
	///	     it is marked as failed. If a test is expected to
	///	     fail, but passes, it is marked as failed.
	///
	/// \return The number of tests that failed.
	size_t	Failing() const;

	/// \brief Total is the number of tests registered.
	size_t	Total() const;

	void	Failed();
	void	AddTest(size_t testCount = 0);
	void	Reset(size_t testCount = 0);

	void	Start();
	void	End();
	std::chrono::duration<double, std::milli>
		Elapsed() const;

	Report();
private:
	size_t	failing{};
	size_t	total{};

	std::chrono::time_point<std::chrono::steady_clock>	start;
	std::chrono::time_point<std::chrono::steady_clock>	end;
};


std::ostream &operator<<(std::ostream &os, const Report &report);
} // end namespace sctest


#endif
