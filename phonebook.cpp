//
// Created by kyle on 2023-10-10.
//

#include <iostream>
#include <string>
using namespace std;

#include "Arena.h"
#include "Dictionary.h"
using namespace klib;

static const char	*defaultPhonebook = "pb.dat";

static void
usage(ostream &os, int exc)
{
	os << "phonebook is a tool for interacting with phonebook files.\n";
	os << "\nThe default filename is pb.dat.\n\n";
	os << "Usage:\n";
	os << "\tphonebook [-f file] list\n";
	os << "\tphonebook [-f file] del key\n";
	os << "\tphonebook [-f file] has key\n";
	os << "\tphonebook [-f file] get key value\n";
	os << "\tphonebook [-f file] put key value\n";
	os << "\n";

	exit(exc);
}

int
main(int argc, char *argv[])
{
	Arena	 	 arena;
	Dictionary	 pb(arena);
	char		*pbFile = (char *)defaultPhonebook;
	int		 optind = 1;

	for (optind; optind < argc; optind++) {
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

	cout << "[+] loading phonebook from " << pbFile << "\n";
	if (arena.Open(pbFile) != 0) {
		cerr << "Failed to open " << pbFile << "\n";
		exit(1);
	}

	auto command = string(argv[optind++]);

	if (command == "list") {
		cout << pb << "\n";
	} else if (command == "del") {

	}

}