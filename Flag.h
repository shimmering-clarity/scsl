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
#include <optional>
#include <variant>
#include <vector>


namespace klib {



enum class FlagType : uint8_t {
	Unknown		= 0,
	Boolean		= 1,
	Integer		= 2,
	UnsignedInteger	= 3,
	String		= 4,
};


enum class ParseStatus : uint8_t {
	OK		= 0,
	EndOfFlags	= 1,
	NotRegistered	= 2,	
	NotEnoughArgs	= 3,
};


typedef std::variant<
	std::string *,
	bool *,
	int64_t *,
	uint64_t *> FlagValue;


struct Flag {
	Flag(FlagType fType, std::string fName, std::string fDescription);

	FlagType	Type;
	bool		WasSet;
	std::string	Name;
	std::string	Description;
	FlagValue	Value;
};
typedef std::optional<FlagValue>	OptFlagValue;


class Flags {
public:
	Flags(std::string fName);
	Flags(std::string fName, std::string fDescription);

	bool	Register(std::string fName, 
			 FlagType fType,
			 std::string fDescription);
	Flag	*Lookup(std::string fName);
	OptFlagValue
		 ValueOf(std::string fName);

	int	 Parse(int argc, char **argv);

	void	 Usage(std::ostream &os, int exitCode);

	size_t	 			NumArgs();
	std::vector<std::string>	Args();
	std::string			Arg(int index);

	bool	GetBool(std::string name, bool &flagValue);
	// bool	GetUnsignedInteger(std::string name, uint64_t &flagValue)
	// bool	GetInteger(std::string name, int64_t &flagValue)
	// bool	GetString(std::string name, std::string &flagValue)


private:
	ParseStatus		parseArg(int argc, char **argv, int &index);

	std::string			name;
	std::string			description;
	std::vector<std::string>	args;
	std::map<std::string, Flag *>	flags;
};



} // namespace klib
