///
/// \file Commander.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-10
/// \brief Subprogram tooling.
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

#include <scsl/Commander.h>


namespace scsl {


Subcommand::Status
Subcommand::Run(std::vector<std::string> args)
{
	auto argc = args.size();
	if (argc < this->requiredArgs) {
		std::cerr << "[!] " << this->command << " expects ";
		std::cerr << this->requiredArgs << " args, but was given ";
		std::cerr << argc << " args.\n";
		return Subcommand::Status::NotEnoughArgs;
	}
	if (this->fn(args)) {
		return Subcommand::Status::OK;
	}

	return Subcommand::Status::Failed;
}

Commander::Commander()
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
Commander::Run(std::string command, std::vector<std::string> args)
{
	if (this->cmap.count(command) != 1) {
		return Subcommand::Status::CommandNotRegistered;
	}

	auto scmd = this->cmap[command];
	return scmd->Run(std::move(args));
}


} // namespace scsl