///
/// \file src/sl/Flags.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-12
/// \brief Flag defines a command-line flag parser.
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
#include <regex>
#include <utility>

#include <vector>
#include <scsl/Flags.h>
#include <scsl/StringUtil.h>


namespace scsl {


static const std::regex isFlag("^--?[a-zA-Z0-9][a-zA-Z0-9-_]*$",
			       std::regex_constants::nosubs|std::regex_constants::optimize);

std::string
Flags::ParseStatusToString(ParseStatus status)
{
	switch (status) {
	case ParseStatus::OK:
		return "OK";
	case ParseStatus::EndOfFlags:
		return "end of flags";
	case ParseStatus::NotRegistered:
		return "flag not registered";
	case ParseStatus::NotEnoughArgs:
		return "not enough args passed to flags";
	default:
		return "unknown/unspecified parse error";
	}
}


Flag *
NewFlag(std::string fName, FlagType fType, std::string fDescription)
{
	auto *flag = new Flag;

	flag->Type        = fType;
	flag->WasSet      = false;
	flag->Name        = std::move(fName);
	flag->Description = std::move(fDescription);
	flag->Value       = FlagValue{};

	return flag;
}


Flags::Flags(std::string fName)
    : name(std::move(fName))
{}


Flags::Flags(std::string fName, std::string fDescription)
    : name(std::move(fName)), description(std::move(fDescription))
{
}


Flags::~Flags()
{
	for (auto &flag : this->flags) {
		if (flag.second->Type == FlagType::String) {
			delete flag.second->Value.s;
		}
		delete flag.second;
	}
}


bool
Flags::Register(std::string fName, FlagType fType, std::string fDescription)
{
	if (!std::regex_search(fName, isFlag) || fName == "-h") {
		return false;
	}

	if (this->flags.count(fName) != 0) {
		return false;
	}

	auto *flag = NewFlag(fName, fType, std::move(fDescription));
	assert(flag != nullptr);
	this->flags[fName] = flag;
	return true;
}


bool
Flags::Register(std::string fName, bool defaultValue, std::string fDescription)
{
	if (!this->Register(fName, FlagType::Boolean, std::move(fDescription))) {
		return false;
	}

	this->flags[fName]->Value.b = defaultValue;
	return true;
}


bool
Flags::Register(std::string fName, int defaultValue, std::string fDescription)
{
	if (!this->Register(fName, FlagType::Integer, std::move(fDescription))) {
		return false;
	}

	this->flags[fName]->Value.i = defaultValue;
	return true;
}


bool
Flags::Register(std::string fName, unsigned int defaultValue, std::string fDescription)
{
	if (!this->Register(fName, FlagType::UnsignedInteger, std::move(fDescription))) {
		return false;
	}
	assert(this->flags.count(fName) != 0);
	assert(this->flags[fName] != nullptr);

	this->flags[fName]->Value.u = defaultValue;
	return true;
}


bool
Flags::Register(std::string fName, size_t defaultValue, std::string fDescription)
{
	if (!this->Register(fName, FlagType::SizeT, std::move(fDescription))) {
		return false;
	}

	this->flags[fName]->Value.size = defaultValue;
	return true;
}


bool
Flags::Register(std::string fName, std::string defaultValue, std::string fDescription)
{
	if (!this->Register(fName, FlagType::String, std::move(fDescription))) {
		return false;
	}

	this->flags[fName]->Value.s = new std::string(std::move(defaultValue));
	return true;
}


size_t
Flags::Size()
{
	return this->flags.size();
}


Flag *
Flags::Lookup(std::string fName)
{
	if (this->flags.count(fName) == 0) {
		return nullptr;
	}

	return this->flags[fName];
}


bool
Flags::ValueOf(std::string fName, FlagValue &value)
{
	if (this->flags.count(fName) != 0U) {
		return false;
	}

	value = this->flags[fName]->Value;
	return true;
}


Flags::ParseStatus
Flags::parseArg(int argc, char **argv, int &index)
{
	std::string arg(argv[index]);
	string::TrimWhitespace(arg);

	index++;
	if (!std::regex_search(arg, isFlag)) {
		return ParseStatus::EndOfFlags;
	}

	if (this->flags.count(arg) == 0) {
		if (arg == "-h" || arg == "--help") {
			Usage(std::cout, 0);
		}
		return ParseStatus::NotRegistered;
	}

	auto *flag = flags[arg];
	if ((flag->Type != FlagType::Boolean) && index == argc) {
		return ParseStatus::NotEnoughArgs;
	}

	switch (flag->Type) {
	case FlagType::Boolean:
		flag->WasSet  = true;
		flag->Value.b = true;
		return ParseStatus::OK;
	case FlagType::Integer:
		flag->WasSet  = true;
		flag->Value.i = std::stoi(argv[++index], nullptr, 0);
		return ParseStatus::OK;
	case FlagType::UnsignedInteger:
		flag->WasSet  = true;
		flag->Value.u = static_cast<unsigned int>(std::stoi(argv[index++], nullptr, 0));
		return ParseStatus::OK;
	case FlagType::String:
		flag->WasSet  = true;
		flag->Value.s = new std::string(argv[index++]);
		return ParseStatus::OK;
	case FlagType::SizeT:
		flag->WasSet     = true;
		flag->Value.size = static_cast<size_t>(std::stoull(argv[index++]));
		return ParseStatus::OK;
	default:
#if defined(NDEBUG) or defined(SCSL_NOEXCEPT)
		return ParseStatus::Unknown;
#else
		throw std::runtime_error("unhandled type");
#endif
	}

	return ParseStatus::OK;
}


Flags::ParseStatus
Flags::Parse(int argc, char **argv, bool skipFirst)
{
	int index = 0;
	if (skipFirst) {
		index = 1;
	}

	while (index != argc) {
		auto result = this->parseArg(argc, argv, index);
		switch (result) {
		case ParseStatus::OK:
			continue;

		case ParseStatus::EndOfFlags:
			while (index < argc) {
				this->args.emplace_back(argv[index]);
				index++;
			}
			continue;
		case ParseStatus::NotEnoughArgs:
		case ParseStatus::NotRegistered:
			// fall through //
			return result;
		default:
#if defined(NDEBUG) or defined(SCSL_NOEXCEPT)
			return ParseStatus::Unknown;
#else
			throw std::runtime_error("unhandled parse state");
#endif
		}

	}

	return ParseStatus::OK;
}


void
Flags::Usage(std::ostream &os, int exitCode)
{
	os << this->name << ":\t";
	auto indent = this->name.size() + 7;

	string::WriteTabIndented(os, description, 72 - indent, indent / 8, false);
	os << "\n\n";

	for (const auto &pair : this->flags) {
		auto argLine = "\t" + pair.first;
		switch (pair.second->Type) {
		case FlagType::Boolean:
			argLine += "\t\t";
			break;
		case FlagType::Integer:
			argLine += "int\t\t";
			break;
		case FlagType::UnsignedInteger:
			argLine += "uint\t\t";
			break;
		case FlagType::SizeT:
			argLine += "size_t\t";
			break;
		case FlagType::String:
			argLine += "string\t";
			break;
		case FlagType::Unknown:
			// fallthrough
		default:
#ifdef SCSL_NOEXCEPT
			abort();
#else
			throw std::runtime_error("unhandled parsing error - this is a bug");
#endif
			break;
		}

		os << argLine;
		indent = argLine.size();
		string::WriteTabIndented(os, pair.second->Description,
				       72-indent, (indent/8)+2, false);
	}

	os << "\n";
	exit(exitCode);
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


std::string
Flags::Arg(size_t i)
{
	if (i >= this->args.size()) {
		throw std::out_of_range("index is out of range");
	}

	return this->args[i];
}


Flag *
Flags::checkGetArg(std::string& fName, FlagType eType)
{
	if (this->flags[fName] == nullptr) {
		return nullptr;
	}

	auto *flag = this->flags[fName];
	if (flag == nullptr) {
		return nullptr;
	}

	if (flag->Type != eType) {
		return nullptr;
	}

	return flag;
}


bool
Flags::GetBool(std::string fName, bool &flagValue)
{
	auto *flag = this->checkGetArg(fName, FlagType::Boolean);

	flagValue = flag->Value.b;
	return flag->WasSet;
}


bool
Flags::GetInteger(std::string fName, int &flagValue)
{
	auto *flag = this->checkGetArg(fName, FlagType::Integer);

	flagValue = flag->Value.i;
	return flag->WasSet;
}


bool
Flags::GetUnsignedInteger(std::string fName, unsigned int &flagValue)
{
	auto *flag = this->checkGetArg(fName, FlagType::UnsignedInteger);

	flagValue = flag->Value.u;
	return flag->WasSet;
}


bool
Flags::GetSizeT(std::string fName, std::size_t &flagValue)
{
	auto *flag = this->checkGetArg(fName, FlagType::SizeT);

	flagValue = flag->Value.size;
	return flag->WasSet;
}


bool
Flags::GetString(std::string fName, std::string &flagValue)
{
	auto *flag = this->checkGetArg(fName, FlagType::String);

	if (flag->Value.s == nullptr) {
		return false;
	}

	flagValue = *(flag->Value.s);
	return flag->WasSet;
}


} // namespace scsl

