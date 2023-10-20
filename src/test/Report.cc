///
/// \file src/test/Report.cpp
/// \author Kyle Isom
/// \date 2017-06-07
///
/// \brief Defines a Report structure that contains information about
///        the results of unit tests.
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

#include <chrono>
#include <iomanip>
#include <ostream>

#include <sctest/Report.h>



namespace sctest {


Report::Report()
{
	this->Reset(0);
}


size_t
Report::Failing() const
{
	return this->failing;
}


size_t
Report::Total() const
{
	return this->total;
}


void
Report::Failed()
{
	this->failing++;
}


void
Report::AddTest(size_t testCount)
{
	this->total += testCount;
}


void
Report::Reset(size_t testCount)
{
	auto now = std::chrono::steady_clock::now();
	this->total = testCount;
	this->failing = 0;

	this->Start();
	this->end = now;
}



void
Report::Start()
{
	this->start = std::chrono::steady_clock::now();
}


void
Report::End()
{
	this->end = std::chrono::steady_clock::now();
}


std::chrono::duration<double, std::milli>
Report::Elapsed() const
{
	return this->end - this->start;
}


std::ostream&
operator<<(std::ostream &os, const Report &report)
{
	auto elapsed = report.Elapsed();

	os << report.Total() - report.Failing() << "/"
	   << report.Total() << " tests passed in "
	   << std::setw(3) << elapsed.count() << "ms";

	return os;
}



} // end namespace sctest