//
// Project: scccl
// File: include/test/checks.h
// Author: Kyle Isom
// Date: 2017-06-05
// Namespace: test.
//
// checks.h defines a number of macros (which are global in scope) for
// use in test functions that return bools.
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
#ifndef __SCTEST_CHECKS_H
#define __SCTEST_CHECKS_H

#include <scccl/math/math.h>


namespace sctest {


// The following checks are designed as shortcuts that just return false on certain
// conditions.
#define SCTEST_CHECK(x)			if (!(x))	{ return false; }
#define SCTEST_CHECK_FALSE(x)		if ((x))	{ return false; }
#define SCTEST_CHECK_EQ(x, y)		if ((x) != (y))	{ return false; }
#define SCTEST_CHECK_NE(x, y)		if ((x) == (y))	{ return false; }
#define SCTEST_CHECK_ZERO(x)		if ((x) != 0)	{ return false; }
#define SCTEST_CHECK_GTZ(x)		if ((x) > 0) 	{ return false; }
#define SCTEST_CHECK_GEZ(x)		if ((x) >= 0)	{ return false; }
#define SCTEST_CHECK_LEZ(x)		if ((x) <= 0)	{ return false; }
#define SCTEST_CHECK_LTZ(x)		if ((x) < 0)	{ return false; }
#define SCTEST_CHECK_FEQ(x, y)		{ float eps; scmath::DefaultEpsilon(eps); if (!scmath::WithinTolerance((x), (y), eps)) { return false; }}
#define SCTEST_CHECK_DEQ(x, y)		{ double eps; scmath::DefaultEpsilon(eps); if (!scmath::WithinTolerance((x), (y), eps)) { return false; }}

} // namespace test


#endif
