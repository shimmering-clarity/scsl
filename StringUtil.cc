#include <iostream>
#include <sstream>

#include "StringUtil.h"


namespace scsl {
/// namespace U contains utilities.
namespace U {

/// namespace S contains string-related functions.
namespace S {


/*
std::vector<std::string>
SplitKeyValuePair(std::string line, std::string delimiter)
{
	std::string key;
	std::string val;

	auto pos = line.find(delimiter);
	if (pos == std::string::npos) {
		key = line;
		val = "";
	} else {
		key = line.substr(0, pos);
		val = line.substr(pos + 1, line.size() - pos - 2);
	}

	TrimWhitespace(key);
	TrimWhitespace(val);
	return {key, val};
}


std::vector<std::string>
SplitKeyValuePair(std::string line, char delimiter)
{
	std::string key;
	std::string val;

	auto pos = line.find(delimiter);
	if (pos == std::string::npos) {
		key = line;
		val = "";
	} else {
		key = line.substr(0, pos);
		val = line.substr(pos + 1, line.size() - pos - 2);
	}

	TrimWhitespace(key);
	TrimWhitespace(val);
	return {key, val};
}
*/


void
TrimLeadingWhitespace(std::string &s)
{
	s.erase(s.begin(),
		std::find_if(s.begin(), s.end(),
			     [](unsigned char ch) {
				 return !std::isspace(ch);
			     }));
}


void
TrimTrailingWhitespace(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
	    return !std::isspace(ch);
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

	for (ss = 0; s.size() != 0 && ss < s.size(); ss++) {
		se = s.find(delim, ss);
		if ((maxCount > 0) && (parts.size() == maxCount - 1)) {
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


std::ostream &
VectorToString(std::ostream &os, const std::vector<std::string> &svec)
{
	os << "(";
	os << svec.size();
	os << ")";
	os << "{";

	for (size_t i = 0; i < svec.size(); i++) {
		if (i > 0) os << ", ";
		os << svec[i];
	}

	os << "}";
	return os;
}


std::string
VectorToString(const std::vector<std::string> &svec)
{
	std::stringstream	ss;

	VectorToString(ss, svec);
	return ss.str();
}


} // namespace S
} // namespace U
} // namespace scsl
