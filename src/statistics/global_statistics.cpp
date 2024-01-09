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

#include "statistics/global_statistics.h"

// This
#include "resource/resource_provider.h"
#include "utils/types/project_variables.h"
#include "utils/types/structs.h"

// C++
#include <iostream>
#include <map>
#include <regex>

// OpenCV
#include <opencv2/core/persistence.hpp>

namespace ProjectName {

	// ----------------------------------------------------------------------------
	GlobalStatistics::GlobalStatistics(const std::string& output_ref_folder_path)
	{
		try
		{
			trainStatistics(output_ref_folder_path);
		}
		catch (const std::exception& e)
		{
			std::cerr << __func__ << " " << e.what() << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	std::map<std::string, std::string> GlobalStatistics::authenticationResults() const noexcept
	{
		std::map<std::string, std::vector<int>> nb_tp_by_pattern;
		std::map<std::string, std::vector<int>>::iterator it_map;

		for (const TrainStatistics& train_statistics : m_statistics)
			for (const TestStatistics& test_statistics : train_statistics.m_test_statistics)
				for (const PatternStatistics& pattern_statistics : test_statistics.m_pattern_statistics)
				{
					std::string map_key = pattern_statistics.m_video_name;

					it_map = nb_tp_by_pattern.find(map_key);
					if (it_map == nb_tp_by_pattern.end())
						nb_tp_by_pattern[map_key] = std::vector<int>();

					nb_tp_by_pattern[map_key].push_back(pattern_statistics.m_tp);
				}

		std::map<std::string, std::string> authentication_results;
		
		it_map = nb_tp_by_pattern.begin();
		for (; it_map != nb_tp_by_pattern.end(); it_map++)
		{
			std::string map_key = it_map->first;
			std::string doc_label = "genuine";

			const std::vector<int>& results = it_map->second;
			for(size_t i = 0; i < results.size(); i++)
				if(results[i] < 10)
					doc_label = "counterfeit";
			
			authentication_results[map_key] = doc_label;
		}

		return authentication_results;
	}

	// ----------------------------------------------------------------------------
	std::map<std::string, PairDouble> GlobalStatistics::meanPrecisionForCameraType() const noexcept
	{
		std::map<std::string, Tuple2DoubleInt> sum_stats;
		std::map<std::string, Tuple2DoubleInt>::iterator it_sum_stats;

		for (const TrainStatistics& train_statistics : m_statistics)
			for (const TestStatistics& test_statistics : train_statistics.m_test_statistics)
				for (const PatternStatistics& pattern_statistics : test_statistics.m_pattern_statistics)
				{
					std::string doc_label = pattern_statistics.m_doc_label;
					std::string camera_type = pattern_statistics.m_camera_type;
					std::string map_key = doc_label + " " + camera_type;

					it_sum_stats = sum_stats.find(map_key);
					if (it_sum_stats == sum_stats.end())
						sum_stats[map_key] = std::make_tuple(0., 0., 0);

					std::get<0>(sum_stats[map_key]) += pattern_statistics.m_precision;
					std::get<1>(sum_stats[map_key]) += pattern_statistics.m_recall;
					std::get<2>(sum_stats[map_key])++;
				}

		int nb_documents = sum_stats.size();
		std::map<std::string, PairDouble> mean_stats;

		it_sum_stats = sum_stats.begin();
		for (; it_sum_stats != sum_stats.end(); it_sum_stats++)
		{
			std::string map_key = it_sum_stats->first;
			double mean_precision = std::get<0>(sum_stats[map_key]) / std::get<2>(sum_stats[map_key]);
			double mean_recall = std::get<1>(sum_stats[map_key]) / std::get<2>(sum_stats[map_key]);

			mean_stats[map_key] = std::make_pair(mean_precision, mean_recall);
		}

		return mean_stats;
	}

	// ----------------------------------------------------------------------------
	std::map<std::string, PairDouble> GlobalStatistics::meanPrecisionForDocument() const noexcept
	{
		std::map<std::string, Tuple2DoubleInt> sum_stats;
		std::map<std::string, Tuple2DoubleInt>::iterator it_sum_stats;

		for (const TrainStatistics& train_statistics : m_statistics)
			for (const TestStatistics& test_statistics : train_statistics.m_test_statistics)
				for (const PatternStatistics& pattern_statistics : test_statistics.m_pattern_statistics)
				{
					std::string doc_label = pattern_statistics.m_doc_label;
					std::string doc_id = pattern_statistics.m_doc_id;
					std::string map_key = doc_label + " " + doc_id;

					it_sum_stats = sum_stats.find(map_key);
					if (it_sum_stats == sum_stats.end())
						sum_stats[map_key] = std::make_tuple(0., 0., 0);

					std::get<0>(sum_stats[map_key]) += pattern_statistics.m_precision;
					std::get<1>(sum_stats[map_key]) += pattern_statistics.m_recall;
					std::get<2>(sum_stats[map_key])++;
				}

		int nb_documents = sum_stats.size();
		std::map<std::string, PairDouble> mean_stats;

		it_sum_stats = sum_stats.begin();
		for (; it_sum_stats != sum_stats.end(); it_sum_stats++)
		{
			std::string map_key = it_sum_stats->first;
			double mean_precision = std::get<0>(sum_stats[map_key]) / std::get<2>(sum_stats[map_key]);
			double mean_recall = std::get<1>(sum_stats[map_key]) / std::get<2>(sum_stats[map_key]);

			mean_stats[map_key] = std::make_pair(mean_precision, mean_recall);
		}

		return mean_stats;
	}

	// ----------------------------------------------------------------------------
	std::map<std::string, PairDouble> GlobalStatistics::meanPrecisionForPattern() const noexcept
	{
		std::map<std::string, Tuple2DoubleInt> sum_stats;
		std::map<std::string, Tuple2DoubleInt>::iterator it_sum_stats;

		for (const TrainStatistics& train_statistics : m_statistics)
			for (const TestStatistics& test_statistics : train_statistics.m_test_statistics)
				for (const PatternStatistics& pattern_statistics : test_statistics.m_pattern_statistics)
				{
					std::string doc_label = pattern_statistics.m_doc_label;
					std::string pattern_num = pattern_statistics.m_pattern_num;
					std::string map_key = doc_label + " " + pattern_num;

					it_sum_stats = sum_stats.find(map_key);
					if (it_sum_stats == sum_stats.end())
						sum_stats[map_key] = std::make_tuple(0., 0., 0);

					std::get<0>(sum_stats[map_key]) += pattern_statistics.m_precision;
					std::get<1>(sum_stats[map_key]) += pattern_statistics.m_recall;
					std::get<2>(sum_stats[map_key])++;
				}

		std::map<std::string, PairDouble> mean_stats;

		it_sum_stats = sum_stats.begin();
		for (; it_sum_stats != sum_stats.end(); it_sum_stats++)
		{
			std::string map_key = it_sum_stats->first;
			double mean_precision = std::get<0>(sum_stats[map_key]) / std::get<2>(sum_stats[map_key]);
			double mean_recall = std::get<1>(sum_stats[map_key]) / std::get<2>(sum_stats[map_key]);

			mean_stats[map_key] = std::make_pair(mean_precision, mean_recall);
		}

		return mean_stats;
	}

	// ----------------------------------------------------------------------------
	double GlobalStatistics::meanTimeForHologram() const noexcept
	{
		double sum_mean_time = 0.;
		int nb_videos = 0;
		std::map<std::string, PairDoubleInt>::iterator it_sum_stats;

		for (const TrainStatistics& train_statistics : m_statistics)
			for (const TestStatistics& test_statistics : train_statistics.m_test_statistics)
			{
				for (const PatternStatistics& pattern_statistics : test_statistics.m_pattern_statistics)
					sum_mean_time += pattern_statistics.m_mean_time;

				nb_videos++;
			}

		return sum_mean_time / nb_videos;
	}

	// ----------------------------------------------------------------------------
	void GlobalStatistics::trainStatistics(const std::string& output_ref_folder_path)
	{
		try
		{
			for (auto& folder : std::filesystem::directory_iterator(output_ref_folder_path))
			{
				const std::regex train_reg{"^(train_)(.)*"};
				if (folder.is_directory() && std::regex_match(folder.path().filename().string(), train_reg))
				{
					std::string train_video_name = folder.path().filename().string();

					m_statistics.push_back(TrainStatistics(train_video_name));
					testStatistics(folder.path().string());
				}
			}
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void GlobalStatistics::testStatistics(const std::string& train_folder_path)
	{
		try
		{
			for (auto& folder : std::filesystem::directory_iterator(train_folder_path))
			{
				const std::regex test_reg{"^(test_)(.)*"};
				if (folder.is_directory() && std::regex_match(folder.path().filename().string(), test_reg))
				{
					std::string test_folder_filename = folder.path().filename().string();
					m_statistics.back().addTestStatistics(TestStatistics{test_folder_filename});

					std::string test_area_folder = folder.path().string() + "/";
					for (auto& pattern_folder : std::filesystem::directory_iterator(test_area_folder))
					{
						std::string pattern_folder_filename = pattern_folder.path().filename().string();
						std::string output_report_path = pattern_folder.path().string() + "/output_report.yml";
						readVideoStatistics(output_report_path);
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void GlobalStatistics::readVideoStatistics(const std::string& output_report_path)
	{
		PatternStatistics stat;
		cv::FileStorage fs{output_report_path, cv::FileStorage::READ};

		if (!fs.isOpened())
		{
			std::cerr << "The file " << output_report_path << " cannot be opened." << std::endl;
			throw std::runtime_error{"The file " + output_report_path + " cannot be opened."};
		}

		fs["precision"] >> stat.m_precision;
		fs["recall"] >> stat.m_recall;
		fs["nb_img"] >> stat.m_nb_img;
		fs["tp"] >> stat.m_tp;
		fs["tn"] >> stat.m_tn;
		fs["fp"] >> stat.m_fp;
		fs["fn"] >> stat.m_fn;
		fs["mean_time"] >> stat.m_mean_time;
		fs["camera_type"] >> stat.m_camera_type;
		fs["doc_id"] >> stat.m_doc_id;
		fs["doc_label"] >> stat.m_doc_label;
		fs["pattern_num"] >> stat.m_pattern_num;
		fs["video_name"] >> stat.m_video_name;

		fs.release();

		m_statistics.back().m_test_statistics.back().m_pattern_statistics.push_back(stat);
	}

} // namespace ProjectName
