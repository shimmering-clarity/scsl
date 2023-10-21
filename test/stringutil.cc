///
/// \file test/stringutil_test.cc
/// \author kyle 
/// \date 10/14/23
/// \brief Ensure the stringutil functions work.
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that the
/// above copyright notice and this permission notice appear in all copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
/// BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
/// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
/// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
/// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
/// SOFTWARE.
///


#include <functional>
#include <iostream>

#include <scsl/Flags.h>
#include <scsl/StringUtil.h>
#include <sctest/Assert.h>
#include <sctest/SimpleSuite.h>


using namespace scsl;


namespace {


void
TestTrimming(std::string line, std::string lExpected, std::string rExpected, std::string expected)
{
	std::string result;
	std::string message;

	result  = string::TrimLeadingWhitespaceDup(line);
	message = "TrimLeadingDup(\"" + line + "\"): '" + result + "'";
	sctest::Assert(result == lExpected, message);

	result  = string::TrimTrailingWhitespaceDup(line);
	message = "TrimTrailingDup(\"" + line + "\"): '" + result + "'";
	sctest::Assert(result == rExpected, message);

	result  = string::TrimWhitespaceDup(line);
	message = "TrimDup(\"" + line + "\"): '" + result + "'";
	sctest::Assert(result == expected, message);

	result = line;
	string::TrimLeadingWhitespace(result);
	message = "TrimLeadingDup(\"" + line + "\"): '" + result + "'";
	sctest::Assert(result == lExpected, message);

	result = line;
	string::TrimTrailingWhitespace(result);
	message = "TrimTrailingDup(\"" + line + "\"): '" + result + "'";
	sctest::Assert(result == rExpected, message);

	result = line;
	string::TrimWhitespace(result);
	message = "TrimDup(\"" + line + "\"): '" + result + "'";
	sctest::Assert(result == expected, message);
}


std::function<bool()>
TestSplit(std::string line, std::string delim, size_t maxCount, std::vector<std::string> expected)
{
	return [line, delim, maxCount, expected]() {
	    return string::SplitN(line, delim, maxCount) == expected;

	};
}


bool
TestSplitChar()
{
	auto expected = std::vector<std::string>{"hello", "world"};
	const auto *inputLine = "hello=world\n";
	auto actual = string::SplitKeyValuePair(inputLine, '=');

	return actual == expected;
}


bool
TestWrapping()
{
	std::string testLine = "A much longer line, something that can be tested with WrapText. ";
	testLine += "Does it handle puncuation? I hope so.";

	std::vector<std::string> expected{
	    "A much longer",
	    "line, something",
	    "that can be",
	    "tested with",
	    "WrapText. Does",
	    "it handle",
	    "puncuation? I",
	    "hope so.",
	};

	auto wrapped = string::WrapText(testLine, 16);
	if (wrapped.size() != expected.size()) {
		std::cerr << string::VectorToString(wrapped)
			  << " != "
			  << string::VectorToString(expected)
			  << "\n";
	}

	for (size_t i = 0; i < wrapped.size(); i++) {
		if (wrapped[i] == expected[i]) {
			continue;
		}

		std::cerr << "[" << i << "] \"" << wrapped[i] << "\" != \""
			  << expected[i] << "\"\n";
		return false;
	}

//	string::WriteTabIndented(std::cout, wrapped, 4, true);
	return true;
}


} // anonymous namespace


int
main(int argc, char *argv[])
{
	auto noReport = false;
	auto quiet = false;
	auto *flags = new scsl::Flags("test_orientation",
				     "This test validates various orientation-related components in scmp.");
	flags->Register("-n", false, "don't print the report");
	flags->Register("-q", false, "suppress test output");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
	}

	sctest::SimpleSuite suite;
	flags->GetBool("-n", noReport);
	flags->GetBool("-q", quiet);
	if (quiet) {
		suite.Silence();
	}

	TestTrimming(" foo\t ", "foo\t ", " foo", "foo");
	TestTrimming(" foo\tbar ", "foo\tbar ", " foo\tbar", "foo\tbar");

	suite.AddTest("SplitN(0)", TestSplit("abc:def:ghij:klm", ":", 0,
					     std::vector<std::string>{"abc", "def", "ghij", "klm"}));
	suite.AddTest("SplitN(3)", TestSplit("abc:def:ghij:klm", ":", 3,
					     std::vector<std::string>{"abc", "def", "ghij:klm"}));
	suite.AddTest("SplitN(2)", TestSplit("abc:def:ghij:klm", ":", 2,
					     std::vector<std::string>{"abc", "def:ghij:klm"}));
	suite.AddTest("SplitN(1)", TestSplit("abc:def:ghij:klm", ":", 1,
					     std::vector<std::string>{"abc:def:ghij:klm"}));
	suite.AddTest("SplitN(0) with empty element",
		      TestSplit("abc::def:ghi", ":", 0,
				std::vector<std::string>{"abc", "", "def", "ghi"}));
	suite.AddTest("TestSplitKV(char)", TestSplitChar);
	suite.AddTest("TextWrapping", TestWrapping);

	delete flags;
	auto result = suite.Run();
	if (!noReport) { std::cout << suite.GetReport() << "\n"; }
	return result ? 0 : 1;
}