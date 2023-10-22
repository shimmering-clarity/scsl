///
/// \file test/buffer.cc
/// \author K.Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Unit tests on the scsl::Buffer class.
///
/// \section COPYRIGHT
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
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

#include <iostream>

#include <scsl/Buffer.h>
#include <scsl/Flags.h>
#include <sctest/Checks.h>
#include <sctest/SimpleSuite.h>


using namespace scsl;


bool
bufferTest()
{
	Buffer buffer("hlo, world");
	Buffer helloWorld("hello, world!");
	Buffer goodbyeWorld("goodbye, world");
	Buffer goodbyeCruelWorld("goodbye, cruel world");

	buffer.Insert(1, (uint8_t *) "el", 2);
	SCTEST_CHECK_EQ(buffer.Length(), 12);

	buffer.Append('!');
	SCTEST_CHECK_EQ(buffer, helloWorld);

	buffer.Remove(buffer.Length() - 1);
	SCTEST_CHECK_EQ(buffer.Length(), 12);

	buffer.Remove(0, 5);
	buffer.Insert(0, 'g');
	buffer.Insert(1, (uint8_t *) "oodbye", 6);
	SCTEST_CHECK_EQ(buffer, goodbyeWorld);

	buffer.Insert(9, (uint8_t *)"cruel ", 6);
	buffer.Reclaim();
	SCTEST_CHECK_EQ(buffer.Length(), 0);
	SCTEST_CHECK_EQ(buffer.Capacity(), 0);

	buffer.Append("and now for something completely different...");
	buffer.Resize(128);
	SCTEST_CHECK_EQ(buffer.Capacity(), 128);
	buffer.Trim();
	SCTEST_CHECK_EQ(buffer.Capacity(), 64);

	Buffer	buffer2("and now for something completely different...");
	SCTEST_CHECK_EQ(buffer, buffer2);

	buffer2.Remove(buffer2.Length()-3, 3);
	SCTEST_CHECK_NE(buffer, buffer2);

	return true;
}


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet = false;
	auto flags = new scsl::Flags("test_buffer",
				     "This test validates the Buffer class.");
	flags->Register("-n", false, "don't print the report");
	flags->Register("-q", false, "suppress test output");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
		exit(1);
	}

	sctest::SimpleSuite suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	suite.AddTest("bufferTest", bufferTest);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}
