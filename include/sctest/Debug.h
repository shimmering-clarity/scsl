//
// Project: scccl
// File: include/test/debug.h
// Author: Kyle Isom
// Date: 2017-06-05
// Namespace: test
//
// debug.h defines assertions and other debugging functions.
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
#if 0
// Disabled for now.
#pragma once

#include <iostream>
#ifndef NDEBUG
#include <cstdlib>
#endif

namespace test {

// GenerateCoreDumps should be set at the beginning of the program, before
// multithreading. It is *not* threadsafe.
static bool	GenerateCoreDumps = false;

static void
Assert(bool cond) {
	#ifdef NDEBUG
	std::cout << "Not a debug build, skipping assertion." << std::endl;	
	return;
	#endif

	if (!cond) {
		std::cerr << "Assertion failed in " << __func__ << "(" << __FILE__ << ":" << __LINE__ << ")" << std::endl;
		if (GenerateCoreDumps) {
			std::abort();
		}
		else {
			std::exit(1);
		}
	}
}

}
#endif