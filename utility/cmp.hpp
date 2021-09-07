#pragma once

#include <iomanip>

namespace utility::cmp {
	enum class ReportMode {
		COMPRESS, DECOMPRESS
	};
	
	void reportProgress(__int64 all, __int64 cur, unsigned __int64 cmp, const ReportMode rMode) {
		const auto   all_w      = static_cast<__int64>(floor(log10(static_cast<double>(all))) + 1);
		const double completion = static_cast<double>(cur) / static_cast<double>(all) * 100;
		
		utility::console::setCursorPos(0);
		std::cout
			<< "Processed: "
			<< std::left << std::setw(all_w) << cur << " / " << all << " - "
			<< std::setw(7) << std::fixed << std::setprecision(2) << completion << "%"
			<< " | "
			<< (rMode == ReportMode::COMPRESS ? "Compressed: " : "Decompressed: ") << cmp
			<< std::flush;
	}
}
