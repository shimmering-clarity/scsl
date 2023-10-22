///
/// \file test/flags.cc
/// \author K.Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Demonstration of the scsl::Flags class.
///
/// \note This isn't a test program, and won't be run as part of a
///       normal ctest run. It's meant to show how flags work in
///       practice.
///
/// \section COPYRIGHT
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

#include <iostream>
#include <scsl/Flags.h>
#include <sctest/Assert.h>


using namespace scsl;


int
main(int argc, char *argv[])
{
	bool         testFlag     = false;
	size_t       testSize     = 0;
	unsigned int testUnsigned = 0;
	int          testInteger  = 0;
	std::string  testString;

	auto flags = new Flags("flag_test", "this is a test of the flag functionality. This line is particularly long to make sure the wrapping works.");
	flags->Register("-b", FlagType::Boolean, "test boolean");
	flags->Register("-s", FlagType::String, "test string");
	flags->Register("-u", (unsigned int)42, "test unsigned integer with a long description line. This should trigger multiline text-wrapping.");
	flags->Register("-i", -42, "test integer");
	flags->Register("-size", FlagType::SizeT, "test size_t");
	sctest::Assert(flags->Size() == 5, "flags weren't registered");

	auto status = flags->Parse(argc, argv);

	if (status != Flags::ParseStatus::OK) {
		std::cerr << "failed to parse flags: "
			  << Flags::ParseStatusToString(status) << "\n";
		exit(1);
	}

	auto wasSet = flags->GetBool("-b", testFlag);
	std::cout << "  (bool) test flag was set: " << wasSet << "\n";
	std::cout << "  (bool) test flag value: " << testFlag << "\n";

	wasSet = flags->GetInteger("-i", testInteger);
	std::cout << "   (int) test flag was set: " << wasSet << "\n";
	std::cout << "   (int) test flag value: " << testInteger << "\n";

	wasSet = flags->GetUnsignedInteger("-u", testUnsigned);
	std::cout << "  (uint) test flag was set: " << wasSet << "\n";
	std::cout << "  (uint) test flag value: " << testUnsigned << "\n";

	wasSet = flags->GetSizeT("-size", testSize);
	std::cout << "(size_t) test flag was set: " << wasSet << "\n";
	std::cout << "(size_t) test flag value: " << testSize << "\n";

	wasSet = flags->GetString("-s", testString);
	std::cout << "(string) test flag was set: " << wasSet << "\n";
	std::cout << "(string) test flag value: " << testString << "\n";

	delete flags;
	return 0;
}