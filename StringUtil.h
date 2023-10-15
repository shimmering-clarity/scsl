///
/// \file StringUtil.h
/// \author kyle (kyle@midgard)
/// \created 2023-10-14
/// \brief StringUtil contains string utilities.
///
/// \section COPYRIGHT
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


#include <algorithm>
#include <string>
#include <vector>


#ifndef STRINGUTIL_H
#define STRINGUTIL_H


namespace klib {
/// namespace U contains utilities.

namespace U {

/// namespace S contains string-related functions.
namespace S {


/// Remove any whitespace at the beginning of the string. The string
/// is modified in-place.
void		TrimLeadingWhitespace(std::string &s);

/// Remove any whitespace at the end of the string. The string is
/// modified in-place.
void		TrimTrailingWhitespace(std::string &s);

/// Remove any whitespace at the beginning and end of the string. The
/// string is modified in-place.
void		TrimWhitespace(std::string &s);

/// Remove any whitespace at the beginning of the string. The original
/// string isn't modified, and a copy is returned.
std::string	TrimLeadingWhitespaceDup(std::string s);

/// Remove any whitespace at the end of the string. The original string
/// isn't modified, and a copy is returned.
std::string	TrimTrailingWhitespaceDup(std::string s);

/// Remove any whitespace at the beginning and end of the string. The
/// original string isn't modified, and a copy is returned.
std::string	TrimWhitespaceDup(std::string s);


/// Split a line into key and value pairs. If the delimiter isn't found,
/// the line is returned as the first element in the pair, and the second
/// element will be empty.
///
/// \param line A string representing a line in a file.
/// \param delimiter The string delimiter between the key and value.
/// \return The key and value, or {line, ""}.
std::vector<std::string>	SplitKeyValuePair(std::string line, std::string delimiter);

/// Split a line into key and value pairs. If the delimiter isn't found,
/// the line is returned as the first element in the pair, and the second
/// element will be empty.
///
/// \param line A string representing a line in a file.
/// \param delimiter The character delimiter between the key and value.
/// \return The key and value.
std::vector<std::string>	SplitKeyValuePair(std::string line, char delimiter);


std::vector<std::string>	Split(std::string, std::string delimiter);
std::vector<std::string>	Split(std::string, char delimiter);

std::vector<std::string>	SplitN(std::string, std::string delimiter, size_t maxCount=0);
//std::vector<std::string>	SplitN(std::string, char delimiter, size_t size_t maxCount=0);


std::ostream &VectorToString(std::ostream &os, const std::vector<std::string> &svec);
std::string	VectorToString(const std::vector<std::string> &svec);


} // namespace S
} // namespace U
} // namespace klib


#endif // STRINGUTIL_H


