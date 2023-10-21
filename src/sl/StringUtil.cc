///
/// \file StringUtil.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-12
/// \brief Utilities for working with strings.
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

#include <cassert>
#include <iostream>
#include <sstream>

#include <scsl/StringUtil.h>


namespace scsl {
namespace string {


std::vector<std::string>
SplitKeyValuePair(std::string line, std::string delimiter)
{
	auto pair = SplitN(std::move(line), std::move(delimiter), 2);

	if (pair.empty()) {
		return {"", ""};
	}

	if (pair.size() == 1) {
		return {pair[0], ""};
	}

	assert(pair.size() == 2);
	auto key = pair[0];
	auto val = pair[1];

	TrimWhitespace(key);
	TrimWhitespace(val);
	return {key, val};
}


std::vector<std::string>
SplitKeyValuePair(std::string line, char delimiter)
{
	std::string sDelim;

	sDelim.push_back(delimiter);
	return SplitKeyValuePair(std::move(line), sDelim);
}


void
TrimLeadingWhitespace(std::string &s)
{
	s.erase(s.begin(),
		std::find_if(s.begin(), s.end(),
			     [](unsigned char ch) {
				 return std::isspace(ch) == 0;
			     }));
}


void
TrimTrailingWhitespace(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
	    return std::isspace(ch) == 0;
	}).base(), s.end());
}


void
TrimWhitespace(std::string &s)
{
	TrimLeadingWhitespace(s);
	TrimTrailingWhitespace(s);
}


std::string
TrimLeadingWhitespaceDup(std::string s)
{
	TrimLeadingWhitespace(s);
	return s;
}


std::string
TrimTrailingWhitespaceDup(std::string s)
{
	TrimTrailingWhitespace(s);
	return s;
}


std::string
TrimWhitespaceDup(std::string s)
{
	TrimWhitespace(s);
	return s;
}


std::vector<std::string>
SplitN(std::string s, std::string delim, size_t maxCount)
{
	std::vector<std::string> parts;
	size_t                   ss = 0;
	size_t                   se = 0;

	for (ss = 0; !s.empty() && ss < s.size(); ss++) {
		se = s.find(delim, ss);
		if ((maxCount > 0) && (parts.size() == (maxCount - 1))) {
			se = s.size();
		} else if (se == std::string::npos) {
			se = s.size();
		}

		auto length = se - ss;
		parts.push_back(s.substr(ss, length));
		ss = se;
	}

	return parts;
}


std::vector<std::string>
WrapText(std::string& line, size_t lineLength)
{
	std::vector<std::string> wrapped;
	auto                     parts = SplitN(line, " ", 0);

	for (auto &part: parts) {
		TrimWhitespace(part);

	}

	std::string wLine;
	for (auto &word: parts) {
		if (word.empty()) {
			continue;
		}

		if ((wLine.size() + word.size() + 1) > lineLength) {
			wrapped.push_back(wLine);
			wLine.clear();
		}

		if (!wLine.empty()) {
			wLine += " ";
		}
		wLine += word;
	}

	if (!wLine.empty()) {
		wrapped.push_back(wLine);
	}

	return wrapped;
}


void
WriteTabIndented(std::ostream &os, std::vector<std::string> lines,
		 int tabStop, bool indentFirst)
{
	std::string const indent(tabStop, '\t');

	for (size_t i = 0; i < lines.size(); i++) {
		if (i > 0 || indentFirst) {
			os << indent;
		}
		os << lines[i] << "\n";
	}
}


void
WriteTabIndented(std::ostream &os, std::string line, size_t maxLength,
		 int tabStop, bool indentFirst)
{
	auto lines = WrapText(line, maxLength);
	WriteTabIndented(os, std::move(lines), tabStop, indentFirst);
}


std::ostream &
VectorToString(std::ostream &os, const std::vector<std::string> &svec)
{
	os << "(";
	os << svec.size();
	os << ")";
	os << "{";

	for (size_t i = 0; i < svec.size(); i++) {
		if (i > 0) { os << ", "; }
		os << svec[i];
	}

	os << "}";
	return os;
}


std::string
VectorToString(const std::vector<std::string> &svec)
{
	std::stringstream ss;

	VectorToString(ss, svec);
	return ss.str();
}


} // namespace string
} // namespace scsl
