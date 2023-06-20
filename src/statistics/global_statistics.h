#ifndef GLOBAL_STATISTICS_H
#define GLOBAL_STATISTICS_H

// This
#include "utils/types/structs.h"
#include "utils/types/types_cpp.h"

// C++
#include <filesystem>
#include <string>

namespace ProjectName {

	class GlobalStatistics final
	{
	public:
		GlobalStatistics() = delete;
		explicit GlobalStatistics(const std::string& output_report_path);
		GlobalStatistics(const GlobalStatistics&) = default;
		GlobalStatistics(GlobalStatistics&&) noexcept = default;
		virtual ~GlobalStatistics() = default;

		GlobalStatistics& operator=(const GlobalStatistics&) = default;
		GlobalStatistics& operator=(GlobalStatistics&&) noexcept = default;

		// ------
		// GETTER
		GStatistics gStatistics() const noexcept { return m_statistics; }
		// ------

		std::map<std::string, std::string> authenticationResults() const noexcept;
		std::map<std::string, PairDouble> meanPrecisionForCameraType() const noexcept;
		std::map<std::string, PairDouble> meanPrecisionForDocument() const noexcept;
		std::map<std::string, PairDouble> meanPrecisionForPattern() const noexcept;
		double meanTimeForHologram() const noexcept;

	private:
		void trainStatistics(const std::string& output_folder_path); // throw runtime_error
		void testStatistics(const std::string& train_folder_path); // throw runtime_error
		void readVideoStatistics(const std::string& output_report_path); // throw runtime_error

		// ----------------
		// MEMBER VARIABLES
		GStatistics m_statistics;
		// ----------------
	};

} // namespace ProjectName

#endif // GLOBAL_STATISTICS_H
