//
// Created by kyle on 2023-10-10.
//

#include <iostream>
#include <string>
using namespace std;

#include "Arena.h"
#include "Commander.h"
#include "Dictionary.h"
using namespace klib;

static const char	*defaultPhonebook = "pb.dat";
static char		*pbFile = (char *)defaultPhonebook;
static Arena	 	 arena;
static Dictionary	 pb(arena);


static bool
listFiles(int argc, char **argv)
{
	(void)argc; (void)argv;
	cout << "[+] keys in '" << pbFile << "':\n";
	cout << pb;
	return true;
}

static bool
newPhonebook(int argc, char **argv)
{
	(void)argc;

	auto	size = std::stoul(string(argv[0]));
	cout << "[+] create new " << size << "B phonebook '" << pbFile << "'\n";

	return arena.Create(pbFile, size) == 0;
}

static bool
delKey(int argc, char **argv)
{
	(void)argc;
	string key = argv[0];

	cout << "[+] deleting key '" << key << "'\n";
	return pb.Delete(key.c_str(), key.size());
}

static bool
hasKey(int argc, char **argv)
{
	(void)argc;
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
getKey(int argc, char **argv)
{
	(void)argc;
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
putKey(int argc, char **argv)
{
	(void)argc;
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
	int		 optind = 1;

	for (optind = 1; optind < argc; optind++) {
		auto arg = string(argv[optind]);
		if (arg[0] != '-')	break;
		if (arg == "-h")	usage(cout, 0);
		if (arg == "-f") {
			pbFile = argv[optind+1];
			optind++;
			continue;
		}

		usage(cerr, 1);
	}

	if (argc <= 1) {
		usage(cout, 0);
	}

	auto command = string(argv[optind++]);
	Commander	commander;

	commander.Register(Subcommand("list", 0, listFiles));
	commander.Register(Subcommand("new", 1, newPhonebook));
	commander.Register(Subcommand("del", 1, delKey));
	commander.Register(Subcommand("has", 1, hasKey));
	commander.Register(Subcommand("get", 1, getKey));
	commander.Register(Subcommand("put", 2, putKey));

	if (command != "new") {
		cout << "[+] loading phonebook from " << pbFile << "\n";
		if (arena.Open(pbFile) != 0) {
			cerr << "Failed to open " << pbFile << "\n";
			exit(1);
		}
	}

	auto result = commander.Run(command, argc-optind, argv+optind);
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