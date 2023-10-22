///
/// \file phonebook.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-10
/// \brief Commandline tools for interacting with dictionary data file.
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

#include <iostream>
#include <string>
using namespace std;

#include <scsl/Arena.h>
#include <scsl/Commander.h>
#include <scsl/Dictionary.h>
#include <scsl/Flags.h>


using namespace scsl;

static const char	*defaultPhonebook = "pb.dat";
static std::string	 pbFile(defaultPhonebook);
static Arena	 	 arena;
static Dictionary	 pb(arena);


static bool
listFiles(std::vector<std::string> argv)
{
	(void) argv; // provided for interface compatibility.
	cout << "[+] keys in '" << pbFile << "':\n";
	cout << pb;
	return true;
}


static bool
newPhonebook(std::vector<std::string> argv)
{
	auto	size = std::stoul(argv[0]);
	cout << "[+] create new " << size << "B phonebook '" << pbFile << "'\n";

	return arena.Create(pbFile.c_str(), size) == 0;
}


static bool
delKey(std::vector<std::string> argv)
{
	string key = argv[0];

	cout << "[+] deleting key '" << key << "'\n";
	return pb.Delete(key.c_str(), key.size());
}


static bool
hasKey(std::vector<std::string> argv)
{
	string key = argv[0];

	cout << "[+] looking up '" << key << "': ";
	if (pb.Contains(key.c_str(), key.size())) {
		cout << "found\n";
		return true;
	}

	cout << "not found\n";
	return true;
}


static bool
getKey(std::vector<std::string> argv)
{
	TLV::Record	rec;
	auto key = string(argv[0]);

	cout << "[+] key '" << key << "' ";
	if (!pb.Lookup(key.c_str(), key.size(), rec)) {
		cout << "not found\n";
		return false;
	}

	cout << "-> " << rec.Val << "\n";
	return true;
}


static bool
putKey(std::vector<std::string> argv)
{
	auto key = string(argv[0]);
	auto val = string(argv[1]);

	cout << "[+] setting '" << key << "' -> '" << val << "': ";
	if (pb.Set(key.c_str(), key.size(), val.c_str(), val.size()) != 0) {
		cout << "failed\n";
		return false;
	}

	cout << "set\n";
	return true;
}


static void
usage(ostream &os, int exc)
{
	os << "phonebook is a tool for interacting with phonebook files.\n";
	os << "\nThe default filename is pb.dat.\n\n";
	os << "Usage:\n";
	os << "\tphonebook [-f file] list\n";
	os << "\tphonebook [-f file] new size\n";
	os << "\tphonebook [-f file] del key\n";
	os << "\tphonebook [-f file] has key\n";
	os << "\tphonebook [-f file] get key\n";
	os << "\tphonebook [-f file] put key value\n";
	os << "\n";

	exit(exc);
}

int
main(int argc, char *argv[])
{
	bool help = false;
	std::string fileName(pbFile);

	auto *flags = new scsl::Flags("phonebook",
				      "A tool for interacting with Arena-backed dictionary files.");
	flags->Register("-f", pbFile, "path to a phonebook file");
	flags->Register("-h", false, "print a help message");

	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
		exit(1);
	}

	flags->GetString("-f", fileName);
	flags->GetBool("-h", help);

	pbFile = fileName;

	if (help) {
		usage(std::cout, 0);
	}

	if (flags->NumArgs() == 0) {
		usage(std::cerr, 1);
	}

	Commander	commander;
	commander.Register(Subcommand("list", 0, listFiles));
	commander.Register(Subcommand("new", 1, newPhonebook));
	commander.Register(Subcommand("del", 1, delKey));
	commander.Register(Subcommand("has", 1, hasKey));
	commander.Register(Subcommand("get", 1, getKey));
	commander.Register(Subcommand("put", 2, putKey));

	auto command = flags->Arg(0);
	if (command != "new") {
		cout << "[+] loading phonebook from " << pbFile << "\n";
		if (arena.Open(pbFile.c_str()) != 0) {
			cerr << "Failed to open " << pbFile << "\n";
			exit(1);
		}
	}

	auto args = flags->Args();
	args.erase(args.begin());

	auto result = commander.Run(command, args);
	switch (result) {
	case Subcommand::Status::OK:
		std::cout << "[+] OK\n";
		break;
	case Subcommand::Status::NotEnoughArgs:
		usage(cerr, 1);
		break;
	case Subcommand::Status::Failed:
		cerr << "[!] '"<< command << "' failed\n";
		break;
	case Subcommand::Status::CommandNotRegistered:
		cerr << "[!] '" << command << "' not registered.\n";
		usage(cerr, 1);
		break;
	default:
		abort();
	}
}