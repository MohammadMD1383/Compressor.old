#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <numeric>

#include <boost/filesystem.hpp>

#include "utility/console.hpp"
#include "utility/filesystem.hpp"
#include "utility/cmp_mapping.hpp"
#include "utility/cmp.hpp"

class Compressor {
private:
	std::string      iFileName;
	std::string      oFileName;
	std::string      mFileName;
	unsigned __int64 tellProgress;
	unsigned __int8  cmpLevel;

public:
	Compressor(std::string &ifName, const std::string &ofDir,
	           const unsigned __int64 tellProgress = 0, const unsigned __int8 cmpBytes = 255) : iFileName(ifName),
	                                                                                            tellProgress(tellProgress),
	                                                                                            cmpLevel(cmpBytes) {
		oFileName = ofDir.empty() ? ifName + ".compressed" :
		            (boost::filesystem::path(ofDir) / boost::filesystem::path(ifName).filename()).string() + ".compressed";
		mFileName = oFileName + ".mapping";
	}
	
	void report() const noexcept {
		std::cout
			<< std::endl
			<< "File To Compress  : " << iFileName << std::endl
			<< "Output File       : " << oFileName << std::endl
			<< "Mapping File      : " << mFileName << std::endl
			<< "Compression Level : " << +cmpLevel << std::endl
			<< std::endl;
	}
	
	bool compress() const noexcept {
		try {
			std::ifstream inputFile(iFileName, std::ios_base::binary | std::ios_base::in);
			std::ofstream outputFile(oFileName, std::ios_base::binary | std::ios_base::trunc | std::ios_base::out);
			std::ofstream mappingFile(mFileName, std::ios_base::binary | std::ios_base::trunc | std::ios_base::out);
			
			bool             reported  = false;
			unsigned __int64 cmp_count = 0;
			__int64          iFSize    = utility::filesystem::getFileSize(inputFile);
			__int64          cmp_pos;
			unsigned __int16 buffer;
			
			utility::console::setCursorVisibility(false);
			utility::console::setColor(F_LIGHT_BLUE);
			
			// reduce map to requested size
			utility::cmp_mapping::mapping.erase(utility::cmp_mapping::mapping.begin(),
			                                    std::next(utility::cmp_mapping::mapping.begin(), 255 - cmpLevel));
			
			while (inputFile.read(reinterpret_cast<char *>(&buffer), sizeof(__int16))) {
				if (utility::cmp_mapping::mapping.contains(buffer)) {
					cmp_pos = outputFile.tellp();
					mappingFile.write(reinterpret_cast<char *>(&cmp_pos), sizeof(__int64));
					outputFile.write(reinterpret_cast<char *>(&utility::cmp_mapping::mapping[buffer]), sizeof(__int8));
					cmp_count++;
					reported = false;
				} else
					outputFile.write(reinterpret_cast<char *>(&buffer), sizeof(__int16));
				
				if (tellProgress && !(cmp_count % tellProgress) && !reported) {
					utility::cmp::reportProgress(iFSize, inputFile.tellg(), cmp_count, utility::cmp::ReportMode::COMPRESS);
					reported = true;
				}
			}
			
			utility::cmp::reportProgress(iFSize, iFSize, cmp_count, utility::cmp::ReportMode::COMPRESS);
			
			utility::console::setCursorVisibility(true);
			utility::console::setColor(F_WHITE | BLACK);
			
			std::cout << std::endl << std::endl;
			
			inputFile.close();
			outputFile.close();
			mappingFile.close();
		} catch (...) { return false; }
		
		return true;
	}
};