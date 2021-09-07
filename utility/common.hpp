#pragma once

namespace utility::common {
	void setDescription(boost::program_options::options_description &description) {
		description.add_options()
			("help,h", "Show help message\n")
			("version,v", "Show compressor version/details\n")
			("progress,p", boost::program_options::value<unsigned __int64>()->default_value(0)->implicit_value(1000),
			 "Show progress after each (n) compression occurred\n  - values: 0-18,446,744,073,709,551,615\nAffects performance.\n* Not Recommended *\n")
			("compression,c", boost::program_options::value<unsigned __int16>()->default_value(255),
			 "Compression level used to do compression.\nHigher value means more compression but more time.\n  - values: 0-255\n")
			("mode,m", boost::program_options::value<std::string>()->default_value("compress"), "Mode:\n  - compress\n  - decompress\n")
			("file,f", boost::program_options::value<std::string>(), "Input file to compress/decompress\n")
			("output,o", boost::program_options::value<std::string>()->default_value(""), "Output file destination (directory)\n");
	}
	
	const char *getSyntax() {
		return "Usage Syntax:\n"
		       "    Compressor [--mode <mode>] --file <file-name> [--output <output-dir>] [--compression <value>] [--progress <value>]\n"
		       "    Compressor --help\n"
		       "    Compressor --version\n";
	}
	
	const char *getPositionalSyntax() {
		return "Positional:\n"
		       "    Compressor <mode> <file-name> <output-dir> <compression-level>\n";
	}
	
	void setPositionalDef(boost::program_options::positional_options_description &positionalDef) {
		positionalDef
			.add("mode", 1)
			.add("file", 1)
			.add("output", 1)
			.add("compression", 1);
	}
	
	void parseCommandLine(
		const int argc,
		const char **argv,
		const boost::program_options::options_description &description,
		const boost::program_options::positional_options_description &positionalDef,
		boost::program_options::variables_map &variables
	) {
		boost::program_options::store(
			boost::program_options::command_line_parser(argc, argv).options(description).positional(positionalDef).run(),
			variables
		);
		boost::program_options::notify(variables);
	}
}