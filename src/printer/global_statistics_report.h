#ifndef GLOBAL_STATISTICS_REPORT_H
#define GLOBAL_STATISTICS_REPORT_H

// This
#include "utils/types/structs.h"
#include "utils/types/types_cpp.h"

// C++
#include <filesystem>
#include <string>

namespace ProjectName {
	class GlobalStatistics;

	class GlobalStatisticsReport final
	{

	public:
		explicit GlobalStatisticsReport() = default;
		GlobalStatisticsReport(const GlobalStatisticsReport&) = default;
		GlobalStatisticsReport(GlobalStatisticsReport&&) noexcept = default;
		virtual ~GlobalStatisticsReport() = default;

		GlobalStatisticsReport& operator=(const GlobalStatisticsReport&) = default;
		GlobalStatisticsReport& operator=(GlobalStatisticsReport&&) noexcept = default;

		void write(const std::string& output_folder_path, const GlobalStatistics& global_statistics); // throw std::exception

	private:
		void writeAuthenticationReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics); // throw std::exception
		void writeAuthenticationInfoSummary(std::ofstream& ofs, const std::map<std::string, std::string>& authentication_results); // throw std::exception
		void writeConciseCameraTypeReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics); // throw std::exception
		void writeConciseCameraTypeTestInfoSummary(std::ofstream& ofs, const std::map<std::string, PairDouble>& mean_precision_recall); // throw std::exception
		void writeConciseDocumentReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics); // throw std::exception
		void writeConciseDocumentTestInfoSummary(std::ofstream& ofs, const std::map<std::string, PairDouble>& mean_precision_recall); // throw std::exception
		void writeConcisePatternReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics); // throw std::exception
		void writeConcisePatternTestInfoSummary(std::ofstream& ofs, const std::map<std::string, PairDouble>& pattern_mean_precision) noexcept;
		void writeDetailedReport(const std::string& output_folder_path, const GStatistics& g_statistics); // throw std::exception
		void writeDetailedReportTrainInfo(std::ofstream& ofs, const std::string& train_video_name) noexcept;
		void writeDetailedReportTestHeader(std::ofstream& ofs) noexcept;
		void writeDetailedReportTestInfo(std::ofstream& ofs, const PatternStatistics& stat, const std::string& test_video_name) noexcept;
		void writeMeanTimeHologramReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics);
		void writeMeanTimeHologramInfoSummary(std::ofstream& ofs, const double mean_time_hologram) noexcept;
	};

} // namespace ProjectName

#endif // GLOBAL_STATISTICS_REPORT_H
