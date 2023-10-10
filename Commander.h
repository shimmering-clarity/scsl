///
/// \file Commander.h
/// \author kyle 
/// \created 2023-10-10
/// \brief Subprogram tooling.

#include <map>
#include <functional>
#include <string>
#include <vector>


#ifndef KLIB_COMMANDER_H
#define KLIB_COMMANDER_H

namespace klib {


typedef std::function<bool(int, char **)> CommanderFunc;


class Subcommand {
public:
	enum class Status : uint8_t {
		OK = 0,
		NotEnoughArgs = 1,
		Failed = 2,
		CommandNotRegistered = 3,
	};

	Subcommand(std::string name, int argc, CommanderFunc func)
	    : fn(func), args(argc), command(name)
	{}

	std::string Name() { return this->command; }
	Status Run(int argc, char **argv);
private:
	CommanderFunc fn;
	int args;
	std::string command;
};

/// Commander does this.
///
/// Longer description...
class Commander {
public:
	Commander();

	bool Register(Subcommand scmd);
	Subcommand::Status Run(std::string command, int argc, char **argv);
private:
	std::map<std::string, Subcommand *>	cmap;
};

} // klib


#endif //KLIB_COMMANDER_H
