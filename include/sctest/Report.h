///
/// \file include/sctest/Report.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Unit test reporting class.
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

#ifndef SCTEST_REPORT_H
#define SCTEST_REPORT_H

#include <chrono>

namespace sctest {

/// \brief A Report holds test run results.
///
/// This is designed to work with SimpleSuite, but might be useful
/// for other things.
class Report {
public:
	/// \brief Construct a new Report, zeroed out.
	Report();

	/// \brief Failing returns the count of failed tests.
	///
	/// \details If a test is run and expected to pass, but fails,
	///	     it is marked as failed. If a test is expected to
	///	     fail, but passes, it is marked as failed.
	///
	/// \return The number of tests that failed.
	size_t	Failing() const;

	/// \brief The number of tests that have passed successfully.
	size_t Passing() const;

	/// \brief The number of tests registered.
	size_t	Total() const;

	/// \brief Report a test as having failed.
	void	Failed();

	/// \brief Report a test as having passed.
	void	Passed();

	/// \brief Register more tests in the report.
	///
	/// This is used to track the total number of tests in the
	/// report.
	void	AddTest(size_t testCount = 0);

	/// \brief Reset the internal state.
	///
	/// All fields in the Report will be zeroed out.
	///
	/// \param testCount
	void	Reset(size_t testCount = 0);

	/// \brief Mark the start of test runs.
	///
	/// This is used for tracking how long the tests took to complete.
	void	Start();

	/// \brief Mark the end of test runs.
	///
	/// This is used for tracking how long the tests took to complete.
	void	End();

	/// \brief Retrieve how long the tests took to run.
	///
	/// This only makes sense to run after called to Start and End.
	///
	/// \return The number of milliseconds that have elapsed.
	std::chrono::duration<double, std::milli>
		Elapsed() const;
private:
	size_t	failing;
	size_t	passed;
	size_t	total;

	std::chrono::time_point<std::chrono::steady_clock>	start;
	std::chrono::time_point<std::chrono::steady_clock>	end;
};


std::ostream &operator<<(std::ostream &os, const Report &report);
} // end namespace sctest


#endif
