///
/// \file Checks.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Provides a number of utility macros for testing.
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

#ifndef SCTEST_CHECKS_H
#define SCTEST_CHECKS_H

#include <scmp/Math.h>


namespace sctest {


// The following checks are designed as shortcuts that return false on
// if some condition isn't met.
#define SCTEST_CHECK(x)			if (!(x))	{ return false; }
#define SCTEST_CHECK_FALSE(x)		if ((x))	{ return false; }
#define SCTEST_CHECK_EQ(x, y)		if ((x) != (y))	{ return false; }
#define SCTEST_CHECK_NE(x, y)		if ((x) == (y))	{ return false; }
#define SCTEST_CHECK_FEQ(x, y)		{ float eps; scmp::DefaultEpsilon(eps); if (!scmp::WithinTolerance((x), (y), eps)) { return false; }}
#define SCTEST_CHECK_DEQ(x, y)		{ double eps; scmp::DefaultEpsilon(eps); if (!scmp::WithinTolerance((x), (y), eps)) { return false; }}

#define SCTEST_CHECK_FEQ_EPS(x, y, eps)	{ if (!scmp::WithinTolerance<float>((x), (y), eps)) { return false; }}
#define SCTEST_CHECK_DEQ_EPS(x, y, eps)	{ if (!scmp::WithinTolerance<double>((x), (y), eps)) { return false; }}


} // namespace sctest


#endif
