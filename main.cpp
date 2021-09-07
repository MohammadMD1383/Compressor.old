#define APP_VERSION "Compressor Version: 1.0.0-BETA"

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "Compressor.hpp"
#include "Decompressor.hpp"
#include "utility/common.hpp"

namespace po = boost::program_options;
namespace algo = boost::algorithm;

namespace ucom = utility::common;
namespace uc = utility::console;

#ifdef __DEBUG__
#include <conio.h>
#endif

int main(const int argc, const char **argv) {
	std::ios::sync_with_stdio(false);
	
	po::options_description description("Compressor Help: CommandLine Arguments");
	ucom::setDescription(description);
	
	po::positional_options_description positionalDef;
	ucom::setPositionalDef(positionalDef);
	
	po::variables_map variables;
	ucom::parseCommandLine(argc, argv, description, positionalDef, variables);
	
	if (variables.count("help")) {
		std::cout
			<< std::endl
			<< description
			<< ucom::getSyntax()
			<< std::endl
			<< ucom::getPositionalSyntax()
			<< std::endl;
		return 0;
	}
	
	if (variables.count("version")) {
		std::cout << APP_VERSION << std::endl;
		return 0;
	}
	
	if (variables.count("file")) {
		const bool             isCompress = algo::to_lower_copy(variables["mode"].as<std::string>()) == "compress";
		const unsigned __int64 tellProg   = variables["progress"].as<unsigned __int64>();
		const unsigned __int8  cmpLevel   = static_cast<unsigned __int8>(variables["compression"].as<unsigned __int16>());
		std::string            iFile      = variables["file"].as<std::string>();
		std::string            oFileDir   = variables["output"].as<std::string>();
		
		if (isCompress) {
			Compressor cmp(iFile, oFileDir, tellProg, cmpLevel);
			cmp.report();
			
			if (cmp.compress()) std::cout << uc::colorize(F_GREEN, "Compressing done!") << std::endl;
			else std::cout << uc::colorize(F_RED, "Compressing failed.") << std::endl;
		} else {
			Decompressor dcmp(iFile, oFileDir, tellProg);
			dcmp.report();
			
			if (dcmp.decompress()) std::cout << uc::colorize(F_GREEN, "Decompressing done!") << std::endl;
			else std::cout << uc::colorize(F_RED, "Decompressing failed.") << std::endl;
		}
	} else {
		std::cout << uc::colorize(F_RED, "input file is not specified") << std::endl;
		return -1;
	}
	
	#ifdef __DEBUG__
	_getch();
	#endif
	
	return 0;
}
