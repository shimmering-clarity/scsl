///
/// \file stringutil_test.cc
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


#include <iostream>
#include <sstream>

#include "StringUtil.h"
#include "Test.h"


using namespace scsl;


static void
TestTrimming(std::string line, std::string lExpected, std::string rExpected, std::string expected)
{
	std::string result;
	std::string message;

	result  = U::S::TrimLeadingWhitespaceDup(line);
	message = "TrimLeadingDup(\"" + line + "\"): '" + result + "'";
	TestAssert(result == lExpected, message);

	result  = U::S::TrimTrailingWhitespaceDup(line);
	message = "TrimTrailingDup(\"" + line + "\"): '" + result + "'";
	TestAssert(result == rExpected, message);

	result  = U::S::TrimWhitespaceDup(line);
	message = "TrimDup(\"" + line + "\"): '" + result + "'";
	TestAssert(result == expected, message);

	result = line;
	U::S::TrimLeadingWhitespace(result);
	message = "TrimLeadingDup(\"" + line + "\"): '" + result + "'";
	TestAssert(result == lExpected, message);

	result = line;
	U::S::TrimTrailingWhitespace(result);
	message = "TrimTrailingDup(\"" + line + "\"): '" + result + "'";
	TestAssert(result == rExpected, message);

	result = line;
	U::S::TrimWhitespace(result);
	message = "TrimDup(\"" + line + "\"): '" + result + "'";
	TestAssert(result == expected, message);
}


static std::string
vec2string(std::vector<std::string> v)
{
	std::stringstream ss;

	ss << "(";
	ss << v.size();
	ss << ")";
	ss << "{";

	for (size_t i = 0; i < v.size(); i++) {
		if (i > 0) { ss << ", "; }
		ss << v[i];
	}

	ss << "}";
	return ss.str();
}


static void
TestSplit(std::string line, std::string delim, size_t maxCount, std::vector<std::string> expected)
{
	std::cout << "test split\n";
	std::cout << "\t  line: \"" << line << "\"\n";
	std::cout << "\t delim: \"" << delim << "\"\n";
	std::cout << "\t count: " << maxCount << "\n";
	std::cout << "\texpect: " << vec2string(expected) << "\n";
	auto result = U::S::SplitN(line, delim, maxCount);
	std::cout << "\tresult: " << U::S::VectorToString(result) << "\n";
	TestAssert(result == expected, U::S::VectorToString(result));
	std::cout << "OK!\n";
}


static void
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

	auto wrapped = U::S::WrapText(testLine, 16);
	TestAssert(wrapped.size() == expected.size(),
		   U::S::VectorToString(wrapped) + " != " + U::S::VectorToString(expected));

	for (size_t i = 0; i < wrapped.size(); i++) {
		TestAssert(wrapped[i] == expected[i],
			   "\"" + wrapped[i] + "\" != \"" + expected[i] + "\"");
	}

	U::S::WriteTabIndented(std::cout, wrapped, 4, true);
}


int
main()
{
	TestTrimming(" foo\t ", "foo\t ", " foo", "foo");
	TestTrimming(" foo\tbar ", "foo\tbar ", " foo\tbar", "foo\tbar");

	TestSplit("abc:def:ghij:klm", ":", 0,
		  std::vector<std::string>{"abc", "def", "ghij", "klm"});
	TestSplit("abc:def:ghij:klm", ":", 3,
		  std::vector<std::string>{"abc", "def", "ghij:klm"});
	TestSplit("abc:def:ghij:klm", ":", 2,
		  std::vector<std::string>{"abc", "def:ghij:klm"});
	TestSplit("abc:def:ghij:klm", ":", 1,
		  std::vector<std::string>{"abc:def:ghij:klm"});
	TestSplit("abc::def:ghi", ":", 0,
		  std::vector<std::string>{"abc", "", "def", "ghi"});

	TestWrapping();
}