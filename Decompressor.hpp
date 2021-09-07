#pragma once

#define RAW_DATA_SIZE (1024 * 1024 * 10) // 10 MB

#include <string>

class Decompressor {
private:
	std::string      iFileName;
	std::string      oFileName;
	std::string      mFileName;
	unsigned __int64 tellProgress;

public:
	Decompressor(std::string &iFileName, const std::string &oFileDir, const unsigned __int64 tellProgress) : iFileName(iFileName),
	                                                                                                         mFileName(iFileName + ".mapping"),
	                                                                                                         tellProgress(tellProgress) {
		oFileName = oFileDir.empty() ?
		            utility::filesystem::changeFileType(iFileName, "") :
		            utility::filesystem::changeFileType(
			            (boost::filesystem::path(oFileDir) / boost::filesystem::path(iFileName).filename()).string(),
			            ""
		            );
	}
	
	void report() const noexcept {
		std::cout
			<< std::endl
			<< "File To Decompress : " << iFileName << std::endl
			<< "Mapping File Used  : " << mFileName << std::endl
			<< "Output File        : " << oFileName << std::endl
			<< std::endl;
	}
	
	bool decompress() const noexcept {
		try {
			std::ifstream inputFile(iFileName, std::ios_base::binary | std::ios_base::in);
			std::ifstream mappingFile(mFileName, std::ios_base::binary | std::ios_base::in);
			std::ofstream outputFile(oFileName, std::ios_base::binary | std::ios_base::trunc | std::ios_base::out);
			
			unsigned __int64 dcmp_count = 0;
			__int64          iFSize     = utility::filesystem::getFileSize(inputFile);
			__int64          cmp_pos;
			unsigned __int8  buffer;
			unsigned __int16 newBuffer;
			auto             *rawData   = new unsigned __int8[RAW_DATA_SIZE];
			
			utility::console::setCursorVisibility(false);
			utility::console::setColor(F_LIGHT_BLUE);
			
			while (mappingFile.read(reinterpret_cast<char *>(&cmp_pos), sizeof(__int64))) {
				__int64 dataLength = cmp_pos - inputFile.tellg();
				
				copyRawDataInChunks(inputFile, outputFile, rawData, dataLength);
				
				inputFile.read(reinterpret_cast<char *>(&buffer), sizeof(__int8));
				newBuffer = std::find_if(
					utility::cmp_mapping::mapping.begin(),
					utility::cmp_mapping::mapping.end(),
					[&buffer](const std::pair<unsigned __int16, unsigned __int8> &pair) { return pair.second == buffer; }
				)->first;
				outputFile.write(reinterpret_cast<char *>(&newBuffer), sizeof(__int16));
				
				dcmp_count++;
				if (tellProgress && !(dcmp_count % tellProgress))
					utility::cmp::reportProgress(iFSize, inputFile.tellg(), dcmp_count, utility::cmp::ReportMode::DECOMPRESS);
			}
			
			__int64 dataLength = iFSize - inputFile.tellg();
			copyRawDataInChunks(inputFile, outputFile, rawData, dataLength);
			
			delete[] rawData;
			
			utility::cmp::reportProgress(iFSize, inputFile.tellg(), dcmp_count, utility::cmp::ReportMode::DECOMPRESS);
			
			utility::console::setCursorVisibility(true);
			utility::console::setColor(F_WHITE | BLACK);
			
			std::cout << std::endl << std::endl;
			
			inputFile.close();
			mappingFile.close();
			outputFile.close();
		} catch (...) { return false; }
		
		return true;
	}
	
	inline static void copyRawData(std::ifstream &ifs, std::ofstream &ofs, unsigned __int8 *data_ptr, const __int64 size) {
		ifs.read(reinterpret_cast<char *>(data_ptr), size);
		ofs.write(reinterpret_cast<char *>(data_ptr), size);
	}
	
	static void copyRawDataInChunks(std::ifstream &ifs, std::ofstream &ofs, unsigned __int8 *data_ptr, __int64 dataLength) {
		while (dataLength > RAW_DATA_SIZE) {
			copyRawData(ifs, ofs, data_ptr, RAW_DATA_SIZE);
			dataLength -= RAW_DATA_SIZE;
		}
		copyRawData(ifs, ofs, data_ptr, dataLength);
	}
};
