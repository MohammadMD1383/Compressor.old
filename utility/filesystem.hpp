#pragma once

#include <string>

namespace utility::filesystem {
	std::string changeFileType(std::string s, const char *newType) {
		const size_t &dotPos = s.find_last_of('.');
		return s.replace(dotPos, s.length() - dotPos, newType);
	}
	
	__int64 getFileSize(std::ifstream &fs) {
		fs.seekg(0, std::ios_base::end);
		__int64 end = fs.tellg();
		fs.seekg(0, std::ios_base::beg);
		
		return end;
	}
}