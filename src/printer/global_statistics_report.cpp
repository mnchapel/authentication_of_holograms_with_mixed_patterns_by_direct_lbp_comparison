/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "printer/global_statistics_report.h"

// This
#include "statistics/global_statistics.h"

// C++
#include <fstream>

namespace ProjectName {

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::write(const std::string& output_folder_path, const GlobalStatistics& global_statistics)
	{
		try
		{
			const GStatistics& g_statistics = global_statistics.gStatistics();

			writeAuthenticationReport(output_folder_path, global_statistics);
			writeDetailedReport(output_folder_path, g_statistics);
			writeConcisePatternReport(output_folder_path, global_statistics);
			writeConciseDocumentReport(output_folder_path, global_statistics);
			writeConciseCameraTypeReport(output_folder_path, global_statistics);
			writeMeanTimeHologramReport(output_folder_path, global_statistics);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeAuthenticationReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics)
	{
		std::string authentication_report_path = output_folder_path + "/authentication_report.yml";
		std::ofstream ofs{authentication_report_path, std::ofstream::out};

		if (!ofs.is_open())
			throw std::runtime_error{"The file " + authentication_report_path + " cannot be opened."};

		std::map<std::string, std::string> authentication_results = global_statistics.authenticationResults();
		writeAuthenticationInfoSummary(ofs, authentication_results);

		ofs.close();
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeAuthenticationInfoSummary(std::ofstream& ofs, const std::map<std::string, std::string>& authentication_results)
	{
		ofs.precision(3);

		std::map<std::string, std::string>::const_iterator it = authentication_results.begin();
		for (; it != authentication_results.end(); it++)
		{
			ofs << "Document of the video " << it->first
			<< " is estimated " << it->second << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeConciseCameraTypeReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics)
	{
		std::string concise_camera_type_report_path = output_folder_path + "/concise_camera_type_report.yml";
		std::ofstream ofs{concise_camera_type_report_path, std::ofstream::out};

		if (!ofs.is_open())
			throw std::runtime_error{"The file " + concise_camera_type_report_path + " cannot be opened."};

		std::map<std::string, PairDouble> mean_precision_recall = global_statistics.meanPrecisionForCameraType();
		writeConciseCameraTypeTestInfoSummary(ofs, mean_precision_recall);

		ofs.close();
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeConciseCameraTypeTestInfoSummary(std::ofstream& ofs, const std::map<std::string, PairDouble>& mean_precision_recall)
	{
		ofs.precision(3);

		std::map<std::string, PairDouble>::const_iterator it = mean_precision_recall.begin();
		for (; it != mean_precision_recall.end(); it++)
		{
			ofs << "Camera " << it->first << std::endl;
			ofs << "\tMean precision: " << it->second.first << std::endl;
			ofs << "\tMean recall: " << it->second.second << std::endl;
			ofs << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeConciseDocumentReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics)
	{
		std::string concise_document_report_path = output_folder_path + "/concise_document_report.yml";
		std::ofstream ofs{concise_document_report_path, std::ofstream::out};

		if (!ofs.is_open())
			throw std::runtime_error{"The file " + concise_document_report_path + " cannot be opened."};

		std::map<std::string, PairDouble> mean_precision_recall = global_statistics.meanPrecisionForDocument();
		writeConciseDocumentTestInfoSummary(ofs, mean_precision_recall);

		ofs.close();
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeConciseDocumentTestInfoSummary(std::ofstream& ofs, const std::map<std::string, PairDouble>& mean_precision_recall)
	{
		ofs.precision(3);

		std::map<std::string, PairDouble>::const_iterator it = mean_precision_recall.begin();
		for (; it != mean_precision_recall.end(); it++)
		{
			ofs << "Document " << it->first << std::endl;
			ofs << "\tMean precision: " << it->second.first << std::endl;
			ofs << "\tMean recall: " << it->second.second << std::endl;
			ofs << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeConcisePatternReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics)
	{
		std::string concise_pattern_report_path = output_folder_path + "/concise_pattern_report.yml";
		std::ofstream ofs{concise_pattern_report_path, std::ofstream::out};

		if (!ofs.is_open())
			throw std::runtime_error{"The file " + concise_pattern_report_path + " cannot be opened."};

		std::map<std::string, PairDouble> mean_precision_recall = global_statistics.meanPrecisionForPattern();
		writeConcisePatternTestInfoSummary(ofs, mean_precision_recall);

		ofs.close();
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeConcisePatternTestInfoSummary(std::ofstream& ofs, const std::map<std::string, PairDouble>& mean_precision_recall) noexcept
	{
		ofs.precision(3);

		std::map<std::string, PairDouble>::const_iterator it = mean_precision_recall.begin();
		for (; it != mean_precision_recall.end(); it++)
		{
			ofs << "Pattern " << it->first << std::endl;
			ofs << "\tMean precision: " << it->second.first << std::endl;
			ofs << "\tMean recall: " << it->second.second << std::endl;
			ofs << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeDetailedReport(const std::string& output_folder_path, const GStatistics& g_statistics)
	{
		std::string global_report_path = output_folder_path + "/detailed_report.yml";
		std::ofstream ofs{global_report_path, std::ofstream::out};

		if (!ofs.is_open())
			throw std::runtime_error{"The file " + global_report_path + " cannot be opened."};

		for (const TrainStatistics& train_statistics : g_statistics)
		{
			writeDetailedReportTrainInfo(ofs, train_statistics.m_video_name);
			writeDetailedReportTestHeader(ofs);

			const VecTestStatistics& g_test_statistics = train_statistics.m_test_statistics;
			for (const TestStatistics& test_statistics : g_test_statistics)
			{
				const VecPatternStatistics& g_pattern_statistics = test_statistics.m_pattern_statistics;
				for (const PatternStatistics& pattern_statistics : g_pattern_statistics)
					writeDetailedReportTestInfo(ofs, pattern_statistics, test_statistics.m_video_name);
			}
		}

		ofs.close();
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeDetailedReportTrainInfo(std::ofstream& ofs, const std::string& train_video_name) noexcept
	{
		ofs << std::endl;
		ofs << "train " << train_video_name << std::endl;
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeDetailedReportTestHeader(std::ofstream& ofs) noexcept
	{
		ofs << "            |           |           |   P     R   |  #img |    tp    tn    fp    fn" << std::endl;
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeDetailedReportTestInfo(std::ofstream& ofs, const PatternStatistics& stat, const std::string& test_video_name) noexcept
	{
		ofs.precision(3);
		ofs << std::fixed
				<< stat.m_doc_label << " | "
				<< "video " << test_video_name
				<< " | pattern " << stat.m_pattern_num
				<< " | " << stat.m_precision
				<< "\t" << stat.m_recall
				<< " | " << std::right << std::setw(5) << stat.m_nb_img;

		ofs << std::setprecision(4)
				<< " |"
				<< std::right << std::setw(6) << stat.m_tp
				<< std::right << std::setw(6) << stat.m_tn
				<< std::right << std::setw(6) << stat.m_fp
				<< std::right << std::setw(6) << stat.m_fn << std::endl;
	}
	
	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeMeanTimeHologramReport(const std::string& output_folder_path, const GlobalStatistics& global_statistics) 
	{		
		std::string mean_time_hologram_report_path = output_folder_path + "/mean_time_hologram_report.yml";
		std::ofstream ofs{mean_time_hologram_report_path, std::ofstream::out};

		if (!ofs.is_open())
			throw std::runtime_error{"The file " + mean_time_hologram_report_path + " cannot be opened."};

		double mean_time_hologram = global_statistics.meanTimeForHologram();
		writeMeanTimeHologramInfoSummary(ofs, mean_time_hologram);

		ofs.close();
	}

	// ----------------------------------------------------------------------------
	void GlobalStatisticsReport::writeMeanTimeHologramInfoSummary(std::ofstream& ofs, const double mean_time_hologram) noexcept
	{
		ofs.precision(3);
		
		ofs << "Mean time hologram: " << mean_time_hologram << std::endl;
		ofs << std::endl;
	}

} // namespace ProjectName
