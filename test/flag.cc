//
// Created by kyle on 10/15/23.
//


#include <iostream>
#include <scsl/Flag.h>
#include <sctest/Assert.h>


using namespace scsl;


int
main(int argc, char *argv[])
{
	bool         testFlag     = false;
	size_t       testSize     = 0;
	unsigned int testUnsigned = 0;
	int          testInteger  = 0;
	std::string  testString;

	auto flags = new Flags("flag_test", "this is a test of the flag functionality. This line is particularly long to make sure the wrapping works.");
	flags->Register("-b", FlagType::Boolean, "test boolean");
	flags->Register("-s", FlagType::String, "test string");
	flags->Register("-u", (unsigned int)42, "test unsigned integer with a long description line. This should trigger multiline text-wrapping.");
	flags->Register("-i", -42, "test integer");
	flags->Register("-size", FlagType::SizeT, "test size_t");
	sctest::Assert(flags->Size() == 5, "flags weren't registered");

	auto status = flags->Parse(argc, argv);

	if (status != Flags::ParseStatus::OK) {
		std::cerr << "failed to parse flags: "
			  << Flags::ParseStatusToString(status) << "\n";
		exit(1);
	}

	auto wasSet = flags->GetBool("-b", testFlag);
	std::cout << "  (bool) test flag was set: " << wasSet << "\n";
	std::cout << "  (bool) test flag value: " << testFlag << "\n";

	wasSet = flags->GetInteger("-i", testInteger);
	std::cout << "   (int) test flag was set: " << wasSet << "\n";
	std::cout << "   (int) test flag value: " << testInteger << "\n";

	wasSet = flags->GetUnsignedInteger("-u", testUnsigned);
	std::cout << "  (uint) test flag was set: " << wasSet << "\n";
	std::cout << "  (uint) test flag value: " << testUnsigned << "\n";

	wasSet = flags->GetSizeT("-size", testSize);
	std::cout << "(size_t) test flag was set: " << wasSet << "\n";
	std::cout << "(size_t) test flag value: " << testSize << "\n";

	wasSet = flags->GetString("-s", testString);
	std::cout << "(string) test flag was set: " << wasSet << "\n";
	std::cout << "(string) test flag value: " << testString << "\n";

	delete flags;
	return 0;
}