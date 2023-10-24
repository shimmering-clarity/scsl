///
/// \file include/scsl/SimpleConfig.h
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

#ifndef SCSL_SIMPLECONFIG_H
#define SCSL_SIMPLECONFIG_H


#include <map>
#include <string>
#include <vector>


namespace scsl {


/// \brief SimpleConfig is a basic configuration for projects.
///
/// SimpleConfig is a basic key-value system. It can optionally load
/// key-value pairs from a file, which should consist of key = value
/// lines. Comments may be added by starting the line with a '#'; these
/// lines will be skipped. Comments may have leading whitespace. Any
/// empty lines or lines consisting solely of whitespace will also be
/// skipped.
///
/// When values are retrieved by one of the variants on Get, they are
/// looked up in the following order, assuming `key` and an optional
/// `prefix` set on the config:
///
/// 1. Any cached key-value pairs. Loading a file caches the key-value
///    pairs in the file. The file is not used again, unless another
///    call to Load is made. If the cache has a name for `key`, it will
///    be returned.
/// 2. The value is looked up from the environment. An optional prefix
///    can be set; if so, if there is an environment named
///    `{prefix}{key}`, the value will be cached and it will be
///    returned.
/// 3. If a default value has been provided, it will be cached and
///    returned.
/// 4. If none of the previous steps has provided a value, an empty
///    string will be returned.
///
/// In Go projects, I've used the global config to great success.
/// However, callers may set up an explicit configuration instance.
class SimpleConfig {
public:
#if defined(SCSL_DESKTOP_BUILD)
	/// \brief Load key-value pairs from a file.
	///
	/// \note This operates on the global config.
	///
	/// \param path The path to a config file.
	/// \return 0 if the file was loaded successfully.
	static int 		LoadGlobal(const char *path);

	/// \brief Load key-value pairs from a file.
	///
	/// \note This operates on the global config.
	///
	/// \param path The path to a config file.
	/// \return 0 if the file was loaded successfully.
	static int 		LoadGlobal(std::string &path);

	/// \brief Set the prefix in use by the config.
	///
	/// \note This operates on the global config.
	///
	/// \param prefix The prefix to prepend to the key when looking
	///        up values from the environment.
	static void		SetPrefixGlobal(const std::string &prefix);

	/// \brief Return the keys cached in the config.
	///
	/// Note that this won't returned any non-cached environment
	/// values.
	///
	/// \note This operates on the global config.
	///
	/// \return A list of keys stored under the config.
	static std::vector<std::string>	KeyListGlobal();

	/// \brief Get the value stored for the key from the config.
	///
	/// \note This operates on the global config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \return The value stored under the key, or an empty string.
	static std::string	GetGlobal(std::string &key);

	/// \brief Get the value stored for the key from the config.
	///
	/// \note This operates on the global config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \return The value stored under the key, or an empty string.
	static std::string	GetGlobal(const char *key);

	/// \brief Get the value stored for the key from the config.
	///
	/// \note This operates on the global config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \param defaultValue A default value to cache and use if no
	///        value is stored under the key.
	/// \return The value stored under the key, or the default
	///         value.
	static std::string	GetGlobal(const char *key, const std::string &defaultValue);

	/// \brief Get the value stored for the key from the config.
	///
	/// \note This operates on the global config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \param defaultValue A default value to cache and use if no
	///        value is stored under the key.
	/// \return The value stored under the key, or the default
	///         value.
	static std::string	GetGlobal(std::string &key, const std::string &defaultValue);

	/// \brief Set a configuration value. This will override any
	///        value set.
	static void	 	PutGlobal(std::string &key, const std::string &value);

	/// \brief Set a configuration value. This will override any
	///        value set.
	static void	 	PutGlobal(const char *key, const std::string &value);
#endif

	/// \brief The constructor doesn't need any initialisation.
	SimpleConfig();

	/// \brief The constructor can explicitly set the environment
	///	   prefix.
	explicit SimpleConfig(std::string &prefix);

	/// \brief Load key-value pairs from a file.
	///
	/// \param path The path to a config file.
	/// \return 0 if the file was loaded successfully.
	int				Load(const char *path);

	/// \brief Load key-value pairs from a file.
	///
	/// \param path The path to a config file.
	/// \return 0 if the file was loaded successfully.
	int				Load(std::string& path);

	/// \brief Set the prefix in use by the config.
	///
	/// \param prefix The prefix to prepend to the key when looking
	///        up values from the environment.
	void	SetPrefix(const std::string &prefix);

	/// \brief Return the keys cached in the config.
	///
	/// Note that this won't returned any non-cached environment
	/// values.
	///
	/// \return A list of keys stored under the config.
	std::vector<std::string>	KeyList();

	/// \brief Get the value stored for the key from the config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \return The value stored under the key, or an empty string.
	std::string	Get(std::string &key);

	/// \brief Get the value stored for the key from the config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \return The value stored under the key, or an empty string.
	std::string	Get(const char *key);

	/// \brief Get the value stored for the key from the config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \param defaultValue A default value to cache and use if no
	///        value is stored under the key.
	/// \return The value stored under the key, or the default
	///         value.
	std::string	Get(std::string &key, std::string defaultValue);

	/// \brief Get the value stored for the key from the config.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \param defaultValue A default value to cache and use if no
	///        value is stored under the key.
	/// \return The value stored under the key, or the default
	///         value.
	std::string	Get(const char *key, std::string defaultValue);

	/// \brief Set a configuration value. This will override any
	///        value set.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \param value The value to set.
	void 	Put(std::string &key, const std::string value);

	/// \brief Set a configuration value. This will override any
	///        value set.
	///
	/// \param key The key to look up. See the class documentation
	///        for information on how this is used.
	/// \param value The value to set.
	void	Put(const char *key, const std::string value);
private:
	std::string 				envPrefix;
	std::map<std::string, std::string>	vars;
};


} // namespace scsl


#endif //SCSL_SIMPLECONFIG_H
