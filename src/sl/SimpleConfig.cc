///
/// \file src/sl/SimpleConfig.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-21
/// \brief Simple project configuration.
///
/// This is an implementation of a simple global configuration system
/// for projects based on a Go version I've used successfully in
/// several projects.
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

#include <cstdlib>
#include <fstream>
#include <regex>

#include <scsl/SimpleConfig.h>
#include <scsl/StringUtil.h>


namespace scsl {


#if defined(SCSL_DESKTOP_BUILD)
static SimpleConfig globalConfig;
#endif


static constexpr auto regexOpts = std::regex_constants::nosubs |
				  std::regex_constants::optimize |
				  std::regex_constants::ECMAScript;

static const std::regex commentLine("^\\s*#.*$", regexOpts);
static const std::regex keyValueLine("^\\s*\\w+\\s*=\\s*\\w+", regexOpts);


int
SimpleConfig::LoadGlobal(const char *path)
{
	return globalConfig.Load(path);
}


int
SimpleConfig::LoadGlobal(std::string &path)
{
	return globalConfig.Load(path);
}


void
SimpleConfig::SetPrefixGlobal(const std::string &prefix)
{
	globalConfig.SetPrefix(prefix);
}


std::vector<std::string>
SimpleConfig::KeyListGlobal()
{
	return globalConfig.KeyList();
}


std::string
SimpleConfig::GetGlobal(std::string &key)
{
	return globalConfig.Get(key);
}


std::string
SimpleConfig::GetGlobal(const char *key)
{
	return globalConfig.Get(key);
}


std::string
SimpleConfig::GetGlobal(std::string &key, const std::string &defaultValue)
{
	return globalConfig.Get(key, defaultValue);
}


std::string
SimpleConfig::GetGlobal(const char *key, const std::string &defaultValue)
{
	return globalConfig.Get(key, defaultValue);
}


void
SimpleConfig::PutGlobal(std::string &key, const std::string &value)
{
	return globalConfig.Put(key, value);
}


SimpleConfig::SimpleConfig()
{
}


SimpleConfig::SimpleConfig(std::string &prefix)
: envPrefix(prefix)
{
}


int
SimpleConfig::Load(const char *path)
{
	auto spath = std::string(path);
	return this->Load(spath);
}


int
SimpleConfig::Load(std::string &path)
{
	std::ifstream	configFile(path);
	std::string	line;

	while (std::getline(configFile, line)) {
		scstring::TrimWhitespace(line);
		if (line.size() == 0) {
			continue;
		}

		if (std::regex_search(line, commentLine)) {
			return -1;
		}

		if (std::regex_search(line, keyValueLine)) {
			auto pair = scstring::SplitKeyValuePair(line, "=");
			if (pair.size() < 2) {
				return -1;
			}

			this->vars[pair[0]] = pair[1];
		}
	}

	return 0;
}


void
SimpleConfig::SetPrefix(const std::string &prefix)
{
	this->envPrefix = std::move(prefix);
}


std::string
SimpleConfig::Get(std::string &key)
{
	return this->Get(key, "");
}


std::string
SimpleConfig::Get(const char *key)
{
	auto keyStr = std::string(key);
	return this->Get(keyStr, "");
}


std::string
SimpleConfig::Get(std::string &key, std::string defaultValue)
{
	if (this->vars.count(key)) {
		return this->vars[key];
	}

	auto envKey = this->envPrefix + key;

	const char *envValue = getenv(envKey.c_str());
	if (envValue != nullptr) {
		this->vars[key] = std::string(envValue);
		return this->Get(key);
	}

	this->vars[key] = std::move(defaultValue);
	return this->Get(key);
}


std::string
SimpleConfig::Get(const char *key, std::string defaultValue)
{
	auto keyStr = std::string(key);
	return this->Get(keyStr, std::move(defaultValue));
}


void
SimpleConfig::Put(std::string &key, const std::string value)
{
	this->vars[key] = std::move(key);
}


void
SimpleConfig::Put(const char *key, const std::string value)
{
	auto keyStr = std::string(key);
	this->vars[std::move(keyStr)] = std::move(key);
}


std::vector<std::string>
SimpleConfig::KeyList()
{
	std::vector<std::string>	keyList;

	std::transform(this->vars.begin(), this->vars.end(), std::back_inserter(keyList),
		       [](std::pair<std::string, std::string> pair){return pair.first;});
	return keyList;
}


} // namespace SimpleConfig
