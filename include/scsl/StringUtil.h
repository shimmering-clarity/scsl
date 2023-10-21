///
/// \file include/scsl/StringUtil.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-14
/// \brief Utilities for working with strings.
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


#include <algorithm>
#include <string>
#include <vector>


#ifndef STRINGUTIL_H
#define STRINGUTIL_H


namespace scsl {

/// String-related utility functions.
namespace string {


/// Remove any whitespace At the beginning of the string. The string
/// is modified in-place.
void		TrimLeadingWhitespace(std::string &s);

/// Remove any whitespace At the end of the string. The string is
/// modified in-place.
void		TrimTrailingWhitespace(std::string &s);

/// Remove any whitespace At the beginning and end of the string. The
/// string is modified in-place.
void		TrimWhitespace(std::string &s);

/// Remove any whitespace At the beginning of the string. The original
/// string isn't modified, and a copy is returned.
std::string	TrimLeadingWhitespaceDup(std::string s);

/// Remove any whitespace At the end of the string. The original string
/// isn't modified, and a copy is returned.
std::string	TrimTrailingWhitespaceDup(std::string s);

/// Remove any whitespace At the beginning and end of the string. The
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

/// Split a string into parts based on the delimiter.
///
/// \param s The string that should be split.
/// \param delimiter The string that delimits the parts of the string.
/// \param maxCount The maximum number of parts to split. If 0, there is no
///	   	    limit to the number of parts.
/// \return A vector containing all the parts of the string.
std::vector<std::string>	SplitN(std::string s, std::string delimiter, size_t maxCount=0);

/// WrapText is a very simple wrapping function that breaks the line into
/// lines of At most lineLength characters. It does this by breaking the
/// line into individual words (splitting on whitespace).
std::vector<std::string>	WrapText(std::string& line, size_t lineLength);

/// Write out a vector of lines indented with tabs.
///
/// \param os The output stream to write to.
/// \param lines The lines of text to write.
/// \param tabStop The number of tabs to indent.
/// \param indentFirst Whether the first line should be indented.
void	WriteTabIndented(std::ostream &os, std::vector<std::string> lines,
		 	 int tabStop, bool indentFirst);

/// Wrap a line, then output it to a stream.
///
/// \param os The output stream to write to.
/// \param line The line to wrap and output.
/// \param maxLength The maximum length of each section of text.
/// \param tabStop The number of tabs to indent.
/// \param indentFirst Whether the first line should be indented.
void	WriteTabIndented(std::ostream &os, std::string line, size_t maxLength,
		         int tabStop, bool indentFirst);


/// Write a string vector to the output stream in the same format as
/// VectorToString.
std::ostream &VectorToString(std::ostream &os, const std::vector<std::string> &svec);

/// Return a string represention of a string vector in the form
/// [size]{"foo", "bar", ...}.
std::string   VectorToString(const std::vector<std::string> &svec);


} // namespace string
} // namespace scsl


#endif // STRINGUTIL_H


