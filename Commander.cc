///
/// \file Commander.cpp
/// \author kyle 
/// \date 2023-10-10
///

#include <iostream>
#include "Commander.h"

namespace klib {


Subcommand::Status
Subcommand::Run(int argc, char **argv)
{
	if (argc < this->args) {
		std::cerr << "[!] " << this->command << " expects ";
		std::cerr << this->args << " args, but was given ";
		std::cerr << argc << " args.\n";
		return Subcommand::Status::NotEnoughArgs;
	}
	if (this->fn(argc, argv)) {
		return Subcommand::Status::OK;
	}

	return Subcommand::Status::Failed;
}

Commander::Commander()
    : cmap()
{
	this->cmap.clear();
}

bool
Commander::Register(Subcommand scmd)
{
	if (this->cmap.count(scmd.Name()) > 0) {
		return false;
	}

	auto *pScmd = new Subcommand(scmd);
	this->cmap[scmd.Name()] = pScmd;
	return true;
}


Subcommand::Status
Commander::Run(std::string command, int argc, char **argv)
{
	if (this->cmap.count(command) != 1) {
		return Subcommand::Status::CommandNotRegistered;
	}

	auto scmd = this->cmap[command];
	return scmd->Run(argc, argv);
}


} // klib