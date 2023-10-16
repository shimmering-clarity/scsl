///
/// \file Flag.h
/// \author kyle 
/// \created 2023-10-12
/// \brief Flag declares a command-line flag parser.
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


#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>


namespace scsl {


/// FlagType indicates the value held in a FlagValue.
enum class FlagType : uint8_t {
	Unknown         = 0,
	Boolean         = 1,
	Integer         = 2, ///< int32_t
	UnsignedInteger = 3, ///< uint32_t
	SizeT           = 4, ///< size_t
	String          = 5,
};


enum class ParseStatus : uint8_t {
	Unknown	      = 0,
	OK            = 1,
	EndOfFlags    = 2,
	NotRegistered = 3,
	NotEnoughArgs = 4,
};

std::string
ParseStatusToString(ParseStatus status);


typedef union {
	unsigned int u;
	int          i;
	std::size_t  size;
	std::string  *s;
	bool         b;
} FlagValue;


typedef struct {
	FlagType    Type;
	bool        WasSet;
	std::string Name;
	std::string Description;
	FlagValue   Value;
} Flag;

Flag *
NewFlag(FlagType fType, std::string fName, std::string fDescription);


class Flags {
public:
	Flags(std::string fName);
	Flags(std::string fName, std::string fDescription);

	bool Register(std::string fName,
		      FlagType fType,
		      std::string fDescription);
	bool Register(std::string fName,
		      bool defaultValue,
		      std::string fDescription);
	bool Register(std::string fName,
		      int defaultValue,
		      std::string fDescription);
	bool Register(std::string fName,
		      unsigned int defaultValue,
		      std::string fDescription);
	bool Register(std::string fName,
		      size_t defaultValue,
		      std::string fDescription);
	bool Register(std::string fName,
		      std::string defaultValue,
		      std::string fDescription);
	size_t Size();
	Flag *Lookup(std::string fName);
	bool ValueOf(std::string fName, FlagValue &value);

	ParseStatus Parse(int argc, char **argv);

	void Usage(std::ostream &os, int exitCode);

	size_t NumArgs();
	std::vector<std::string> Args();
	std::string Arg(int index);

	bool GetBool(std::string fName, bool &flagValue);
	bool GetUnsignedInteger(std::string fName, unsigned int &flagValue);
	bool GetInteger(std::string fName, int &flagValue);
	bool GetString(std::string fName, std::string &flagValue);
	bool GetSizeT(std::string fName, std::size_t &flagValue);


private:
	ParseStatus parseArg(int argc, char **argv, int &index);
	Flag *checkGetArg(std::string fName, FlagType eType);

	std::string                   name;
	std::string                   description;
	std::vector<std::string>      args;
	std::map<std::string, Flag *> flags;
};


} // namespace scsl
