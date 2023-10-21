///
/// \file Flag.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2023-10-12
/// \brief Flag declares a command-line flag parser.
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


#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>


namespace scsl {


/// FlagType indicates the value held in a FlagValue.
///
/// \todo When C++17 support is more common, switch to `std::variant`.
enum class FlagType : uint8_t {
	Unknown         = 0, ///< Unsupported value type.
	Boolean         = 1, ///< bool
	Integer         = 2, ///< int32_t
	UnsignedInteger = 3, ///< uint32_t
	SizeT           = 4, ///< size_t
	String          = 5, ///< std::string
};


/// FlagValue holds the value of a command line flag.
typedef union {
	unsigned int u; ///< FlagType::UnsignedInteger
	int          i; ///< FlagType::Integer
	std::size_t  size; ///< FlagType::SizeT
	std::string  *s; ///< FlagType::String
	bool         b; ///< FlagType::Boolean
} FlagValue;


/// Flag describes an individual command-line flag.
typedef struct {
	FlagType    Type; ///< The type of the value in the flag.
	bool        WasSet; ///< The flag was set on the command-line.
	std::string Name; ///< The name of the flag.
	std::string Description; ///< A description of the flag.
	FlagValue   Value; ///< The flag's value.
} Flag;

/// NewFlag is a helper function for constructing a new flag.
///
/// \param fName The name of the flag.
/// \param fType The type of the flag.
/// \param fDescription A description of the flag.
/// \return A pointer to a flag.
Flag	*NewFlag(std::string fName, FlagType fType, std::string fDescription);

/// Flags provides a basic facility for processing command line flags.
///
/// Any remaining arguments after the args are added to the parser as
/// arguments that can be accessed with NumArgs, Args, and Arg.
///
/// \note The parser automatically handles the -h and --help flags by
///       calling Usage(std::cout, 0). The user can override this flag
///       and handle providing help on their own.
///
/// \details
///
/// Arguments are named with their leading dash, e.g. "-f". For example,
///
/// ```c++
/// flags.Register("-f", FlagType::String, "Path to a configuration file.");
/// ```
/// \section Usage
///
/// A short example program is below:
///
/// ```c++
/// int
/// main(int argc, char *argv[])
/// {
/// 	std::string  server = "service.example.com";
/// 	unsigned int port   = 1234;
/// 
/// 	auto flags = new scsl::Flags("example-client",
/// 				     "This interacts with the example.com service.");
/// 	flags->Register("-p", port, "server port");
/// 	flags->Register("-s", server, "hostname to connect to");
/// 
/// 	auto status = flags->Parse(argc, argv);
/// 	if (status != ParseStatus::OK) {
/// 		std::cerr << "failed to parse flags: "
/// 			  << scsl::Flags::ParseStatusToString(status)
/// 			  << "\n";
/// 		exit(1);
/// 	}
/// 
/// 	auto wasSet = flags->GetString("-s", server);
/// 	if (wasSet) {
/// 		std::cout << "hostname override: " << server << "\n";
/// 	}
/// 
/// 	wasSet = flags->GetUnsignedInteger("-p", port);
/// 	if (wasSet) {
/// 		std::cout << "port override: " << port << "\n";
/// 	}
/// 
/// 	std::cout << "connecting to " << server << ":" << port << "\n";
/// 	for (size_t i = 0; i < flags.NumArgs(); i++) {
/// 		std::cout << "\tExecuting command " << flags.Arg(i) << "\n";
/// 	}
/// 	return 0;
/// }
/// ```
///
class Flags {
public:
	/// ParseStatus describes the result of parsing command-line
	/// arguments.
	enum class ParseStatus : uint8_t {
		/// An unknown parsing error occurred. This is a bug,
		/// and users should never see this.
		Unknown	      = 0,

		/// Parsing succeeded.
		OK            = 1,

		/// This is an internal status marking the end of
		/// command line flags.
		EndOfFlags    = 2,

		/// The command line flag provided isn't registered.
		NotRegistered = 3,

		/// Not enough arguments were provided to a flag that
		/// takes an argument. For example, if "-p" expects
		/// a number, and the program was called with just
		/// `./program -p`, this error will be reported.
		NotEnoughArgs = 4,
	};

	/// ParseStatusToString returns a string message describing the
	/// result of parsing command line args.
	static std::string	ParseStatusToString(ParseStatus status);

	/// Create a new flags parser for the named program.
	///
	/// \param fName The program name,
	Flags(std::string fName);

	/// Create a new flags parser for the named program.
	///
	/// \param fName The program name.
	/// \param fDescription A short description of the program.
	Flags(std::string fName, std::string fDescription);

	~Flags();

	/// Register a new command line flag.
	///
	/// \param fName The name of the flag, including a leading dash.
	/// \param fType The type of the argument to parse.
	/// \param fDescription A description of the flag.
	/// \return True if the flag was registered, false if the flag could
	///         not be registered (e.g. a duplicate flag was registered).
	bool Register(std::string fName,
		      FlagType fType,
		      std::string fDescription);

	/// Register a new boolean command line flag with a default value.
	///
	/// \note For booleans, it only makes sense to pass a false default
	///       value, as there is no way to set a boolean flag to false.
	///       This form is provided for compatibility with the other
	///       variants on this method.
	///
	/// \param fName The name of the flag, including a leading dash.
	/// \param defaultValue The default value for the flag.
	/// \param fDescription A short description of the flag.
	/// \return True if the flag was registered, false if the flag could
	///         not be registered (e.g. a duplicate flag was registered).
	bool Register(std::string fName,
		      bool defaultValue,
		      std::string fDescription);

	/// Register a new integer command line flag with a default value.
	///
	/// \param fName The name of the flag, including a leading dash.
	/// \param defaultValue The default value for the flag.
	/// \param fDescription A short description of the flag.
	/// \return True if the flag was registered, false if the flag could
	///         not be registered (e.g. a duplicate flag was registered).
	bool Register(std::string fName,
		      int defaultValue,
		      std::string fDescription);

	/// Register a new unsigned integer command line flag with a default
	/// value.
	///
	/// \param fName The name of the flag, including a leading dash.
	/// \param defaultValue The default value for the flag.
	/// \param fDescription A short description of the flag.
	/// \return True if the flag was registered, false if the flag could
	///         not be registered (e.g. a duplicate flag was registered).
	bool Register(std::string fName,
		      unsigned int defaultValue,
		      std::string fDescription);

	/// Register a new size_t command line flag with a default value.
	///
	/// \param fName The name of the flag, including a leading dash.
	/// \param defaultValue The default value for the flag.
	/// \param fDescription A short description of the flag.
	/// \return True if the flag was registered, false if the flag could
	///         not be registered (e.g. a duplicate flag was registered).
	bool Register(std::string fName,
		      size_t defaultValue,
		      std::string fDescription);

	/// Register a new string command line flag with a default value.
	///
	/// \param fName The name of the flag, including a leading dash.
	/// \param defaultValue The default value for the flag.
	/// \param fDescription A short description of the flag.
	/// \return True if the flag was registered, false if the flag could
	///         not be registered (e.g. a duplicate flag was registered).
	bool Register(std::string fName,
		      std::string defaultValue,
		      std::string fDescription);

	/// Return the number of registered flags.
	size_t Size();

	/// Lookup a flag.
	///
	/// \param fName The flag name.
	/// \return A pointer to flag if registered, or nullptr if the flag
	///         wasn't registered.
	Flag *Lookup(std::string fName);

	/// ValueOf returns the value of the flag in the
	bool ValueOf(std::string fName, FlagValue &value);

	/// Process a list of arguments into flags.
	///
	/// \param argc The number of arguments.
	/// \param argv The arguments passed to the program.
	/// \param skipFirst Flags expects to receive arguments directly
	///        from those passed to `main`, and defaults to skipping
	///        the first argument. Set to false if this is not the
	///        case.
	/// \return
	ParseStatus Parse(int argc, char **argv, bool skipFirst=true);

	/// Print a usage message to the output stream.
	void Usage(std::ostream &os, int exitCode);

	/// Return the number of arguments processed. These are any
	/// remaining elements in argv that are not flags.
	size_t NumArgs();

	/// Return the arguments as a vector.
	std::vector<std::string> Args();

	/// Return a particular argument.
	///
	/// \param index The argument number to extract.
	/// \return The argument At index i. If the index is greater than
	///         the number of arguments present, an out_of_range
	///         exception is thrown.
	std::string Arg(size_t index);

	/// Retrieve the state of a boolean flag.
	///
	/// \param fName The flag to look up.
	/// \param flagValue The value to store.
	/// \return True if the value was set, or false if the value isn't
	///	    a boolean or if it wasn't set.
	bool GetBool(std::string fName, bool &flagValue);

	/// Retrieve the value of an unsigned integer flag.
	///
	/// \param fName The flag to look up.
	/// \param flagValue The value to store.
	/// \return True if the value was set, or false if the value isn't
	///	    an unsigned integer or if it wasn't set.
	bool GetUnsignedInteger(std::string fName, unsigned int &flagValue);

	/// Retrieve the value of an integer flag.
	///
	/// \param fName The flag to look up.
	/// \param flagValue The value to store.
	/// \return True if the value was set, or false if the value isn't
	///	    an integer or if it wasn't set.
	bool GetInteger(std::string fName, int &flagValue);

	/// Retrieve the value of a string flag.
	///
	/// \param fName The flag to look up.
	/// \param flagValue The value to store.
	/// \return True if the value was set, or false if the value isn't
	///	    a string or if it wasn't set.
	bool GetString(std::string fName, std::string &flagValue);

	/// Retrieve the value of a size_t flag.
	///
	/// \param fName The flag to look up.
	/// \param flagValue The value to store.
	/// \return True if the value was set, or false if the value isn't
	///	    a size_t or if it wasn't set.
	bool GetSizeT(std::string fName, std::size_t &flagValue);

private:
	ParseStatus parseArg(int argc, char **argv, int &index);
	Flag *checkGetArg(std::string& fName, FlagType eType);

	std::string                   name;
	std::string                   description;
	std::vector<std::string>      args;
	std::map<std::string, Flag *> flags;
};


} // namespace scsl
