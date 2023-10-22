///
/// \file test/config-explorer.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-21
/// \brief Commandline tools for interacting with simple configurations.
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

#include <scsl/Flags.h>
#include <scsl/SimpleConfig.h>


int
main(int argc, char *argv[])
{
	int retc = 1;
	bool listKeys;
	std::string fileName;
	std::string prefix;
	std::string defaultValue;

	auto *flags = new scsl::Flags("config-explorer",
				      "interact with a simple configuration system");
	flags->Register("-d", "", "set a default value");
	flags->Register("-f", scsl::FlagType::String, "path to a configuration file");
	flags->Register("-l", false, "list cached keys at the end");
	flags->Register("-p", "CX_",
			"prefix for configuration environment variables");
	auto parsed = flags->Parse(argc, argv);
	if (parsed != scsl::Flags::ParseStatus::OK) {
		std::cerr << "Failed to parse flags: "
			  << scsl::Flags::ParseStatusToString(parsed) << "\n";
		exit(1);
	}

	flags->GetString("-d", defaultValue);
	flags->GetString("-f", fileName);
	flags->GetBool("-l", listKeys);
	flags->GetString("-p", prefix);
	scsl::SimpleConfig::SetPrefixGlobal(prefix);

	if (!fileName.empty()) {
		if (scsl::SimpleConfig::LoadGlobal(fileName) != 0) {
			std::cerr << "[!] failed to load " << fileName << "\n";
			return retc;
		}
	}

	for (auto &key : flags->Args()) {
		auto val = scsl::SimpleConfig::GetGlobal(key, defaultValue);
		std::cout << key << ": " << val << "\n";
	}

	if (listKeys) {
		std::cout << "[+] cached keys\n";
		for (auto &key : scsl::SimpleConfig::KeyListGlobal()) {
			std::cout << "\t- " << key << "\n";
		}
	}

	delete flags;
	return retc;
}