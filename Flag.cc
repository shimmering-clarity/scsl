///
/// \file Flag.h
/// \author kyle 
/// \created 2023-10-12
/// \brief Flag defines a command-line flag parser.
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
#include <regex>
#include <vector>

#include "Flag.h"


namespace klib {


static const std::regex	isFlag("^-[a-zA-Z0-9]+$", 
			       std::regex_constants::nosubs);


Flag::Flag(FlagType fType, std::string fName, std::string fDescription)
    : Type(fType), WasSet(false), Name(fName), Description(fDescription)
{
}


Flags::Flags(std::string fName)
	: name(fName), description("")
{
}


Flags::Flags(std::string fName, std::string fDescription)
    : name(fName), description(fDescription)
{
}


bool
Flags::Register(std::string fName, FlagType fType, std::string fDescription)
{
	if (!std::regex_search(fName, isFlag)) {
		return false;
	}

	if (this->flags.count(fName) != 0) {
		return false;
	}

	auto flag = new Flag;
	flag->Type = fType;
	flag->WasSet = false;
	flag->name = name;
	this->desription = fDescription;
	this->flags[fName] = flag;
	return true;
}


Flag *
Flags::Lookup(std::string fName)
{
	if (this->flags.count(fName) == 0) {
		return nullptr;
	}


}


OptFlagValue
Flags::ValueOf(std::string fName)
{
	if (this->flags.count(fName)) {
		return std::nullopt;
	}

	return OptFlagValue(this->flags[fName]->Value);
}



ParseStatus
Flags::parseArg(int argc, char **argv, int &index)
{
	std::string	arg(argv[index]);

	index++;
	if (!std::regex_search(arg, isFlag)) {
		return ParseStatus::EndOfFlags;
	}

	if (this->flags.count(arg) == 0) {
		return ParseStatus::NotRegistered;
	}

	auto flag = flags[arg];
	if (flag->Type == FlagType::Boolean) {
		flag->WasSet = true;
		flag->Value.b = true;
		return ParseStatus::OK;
	}

	switch (flag->Type) {
	default:
		throw std::runtime_error("not handled");
	}

	return ParseStatus::OK;
}


ParseStatus
Flags::Parse(int argc, char **argv)
{
	int	index = 1;

	while (index != argc) {
		auto result = this->parseArg(argc, argv, index);
		switch (result) {
		case ParseStatus::OK:
			continue;

		case ParseStatus::EndOfFlags:
			while (index < argc) {
				this->args.push_back(std::string(argv[index]));
				index++;
			}
			continue;
		case ParseStatus::EndOfFlags:
		case ParseStatus::NotEnoughArgs:
			// fall through //
			return result;
		default:
			throw runtime_error("unhandled parse state");
		}

	}

	return ParseStatus::OK;
}


size_t
Flags::NumArgs()
{
	return this->args.size();
}


std::vector<std::string>
Flags::Args()
{
	return this->args;
}


bool
Flags::GetBool(std::string name, bool &flagValue)
{
	if (this->flags[name] == 0) {
		return false;
	}

	return std::get<bool>(this->flags[name]->Value);
}


} // namespace klib

