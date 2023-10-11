///
/// \file Commander.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-10
/// \brief Subprogram tooling.
///
/// Commander is tooling for creating subcommand interfaces for command-line
/// programs. For an example, see phonebook.cc.
///
/// The basic idea is to enable writing programs of the form
/// ```
///    $ some_tool subcommand args...
/// ```
///

#include <map>
#include <functional>
#include <string>
#include <vector>


#ifndef KLIB_COMMANDER_H
#define KLIB_COMMANDER_H

namespace klib {


/// CommanderFunc describes a function that can be run in Commander.
///
/// It expects an argument count and a list of arguments.
typedef std::function<bool(int, char **)> CommanderFunc;


/// Subcommands are the individual commands for the program. A Subcommand
/// will check that it has enough arguments before running its function.
class Subcommand {
public:
	/// Status describes the results of running a Subcommand.
	enum class Status : uint8_t {
		/// The subcommand executed correctly.
		OK = 0,
		/// Not enough arguments were supplied to the subcommand.
		NotEnoughArgs = 1,
		/// The subcommand failed to run correctly.
		Failed = 2,
		/// The subcommand hasn't been registered in a Commander
		/// instance.
		CommandNotRegistered = 3,
	};

	/// A Subcommand is initialized with a name, the number of arguments
	/// it requires, and a function to run.
	///
	/// \param name The subcommand name; this is the name that will select this command.
	/// \param argc The minimum number of arguments required by this subcommand.
	/// \param func A valid CommanderFunc.
	Subcommand(std::string name, int argc, CommanderFunc func)
	    : fn(func), args(argc), command(name)
	{}

	/// Name returns the name of this subcommand.
	std::string Name() { return this->command; }

	/// Run attempts to run the CommanderFunc for this subcommand.
	///
	/// \param argc The number of arguments supplied.
	/// \param argv The argument list.
	/// \return A Status type indicating the status of running the command.
	Status Run(int argc, char **argv);
private:
	CommanderFunc fn;
	int args;
	std::string command;
};

/// Commander collects subcommands and can run the apppropriate one.
///
/// For example:
/// ```
/// auto command = string(argv[optind++]);
/// Commander      commander;
///
/// commander.Register(Subcommand("list", 0, listFiles));
/// commander.Register(Subcommand("new", 1, newPhonebook));
/// commander.Register(Subcommand("del", 1, delKey));
/// commander.Register(Subcommand("has", 1, hasKey));
/// commander.Register(Subcommand("get", 1, getKey));
/// commander.Register(Subcommand("put", 2, putKey));
///
/// auto result = commander.Run(command, argc-optind, argv+optind);
/// ```
///
class Commander {
public:
	/// A Commander is initialized empty.
	Commander();

	/// Register adds the subcommand. It will be copied into the Commander.
	bool Register(Subcommand scmd);

	/// Try to run a subcommand registered with this Commander.
	Subcommand::Status Run(std::string command, int argc, char **argv);
private:
	std::map<std::string, Subcommand *>	cmap;
};

} // klib


#endif //KLIB_COMMANDER_H
