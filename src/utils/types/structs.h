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

#ifndef STRUCTS_H
#define STRUCTS_H

// This
#include "utils/types/types_cpp.h"
#include "utils/types/types_opencv.h"

// OpenCV
#include <opencv2/ml.hpp>

namespace ProjectName {

	struct TrainResult
	{
		float m_max_y_value = 0.f;
		float m_radius = 0.f;
		double m_max_x_for_norm = 0.;
		double m_max_y_for_norm = 0.;
		double m_ell_cos_angle = 0.;
		double m_ell_sin_angle = 0.;
		PairFloat m_metric_centroid;
		cv::Size2d m_ell_size{0., 0.};
		cv::Point2d m_ell_center{0., 0.};
		cv::Mat m_all_labels_mat;
		cv::Mat m_labels_mat;
		cv::Mat m_lda_eigenvectors;
		cv::Mat m_eigenvectors;
		VecMat m_features;
		VecPairFloat m_metrics;
		VecPairFloat m_all_metrics;
		VecScalar m_label_colors;
		VecScalar m_all_label_colors;
	};

	struct LPModelMetric
	{
		LPModelMetric(float dist_to_multi_ref, float percent_non_uniform)
			: m_dist_to_multi_ref{dist_to_multi_ref}
			, m_percent_non_uniform{percent_non_uniform}
		{
		}

		float m_dist_to_multi_ref;
		float m_percent_non_uniform;
	};

	struct AuthResults
	{
		AuthResults() = delete;
		explicit AuthResults(size_t nb_img)
			: m_lp_model_metrics(nb_img, {0.f, 0.f})
			, m_predicted_labels(nb_img, 0)
		{
		}

		std::vector<LPModelMetric> m_lp_model_metrics;
		VecInt m_predicted_labels;
		double m_mean_time;
	};

	struct PatternStatistics
	{
		PatternStatistics()
			: m_precision{0}
			, m_recall{0}
			, m_nb_img{0}
			, m_tp{0}
			, m_tn{0}
			, m_fp{0}
			, m_fn{0}
			, m_mean_time{0.}
			, m_camera_type{""}
			, m_doc_id{""}
			, m_doc_label{""}
			, m_pattern_num{""}
			, m_video_name{""}
		{
		}

		float m_precision;
		float m_recall;
		int m_nb_img;
		int m_tp;
		int m_tn;
		int m_fp;
		int m_fn;
		double m_mean_time;
		std::string m_camera_type;
		std::string m_doc_id;
		std::string m_doc_label;
		std::string m_pattern_num;
		std::string m_video_name;
	};
	using VecPatternStatistics = std::vector<PatternStatistics>;

	struct TestStatistics
	{		
		TestStatistics(const std::string& test_folder)
		{
			m_video_name = test_folder.substr(test_folder.size() - 5, 3);
		}

		size_t nbPatternStatistics() const noexcept
		{
			return m_pattern_statistics.size();
		}

		std::string m_video_name;
		VecPatternStatistics m_pattern_statistics;
	};
	using VecTestStatistics = std::vector<TestStatistics>;

	struct TrainStatistics
	{
		TrainStatistics(const std::string& train_folder)
		{
			m_video_name = train_folder.substr(train_folder.size() - 5, 3);
		}

		void addTestStatistics(const TestStatistics& test_statistics)
		{
			m_test_statistics.push_back(test_statistics);
		}

		size_t nbTestStatistics() const noexcept
		{
			return m_test_statistics.size();
		}

		std::string m_video_name;
		VecTestStatistics m_test_statistics;
	};
	using GStatistics = std::vector<TrainStatistics>;

	struct Circle
	{
		cv::Point m_center;
		int m_radius;

		Circle(const cv::Point& center, int radius)
			: m_center{center}
			, m_radius{radius}
		{
		}
	};
	using VecCircle = std::vector<Circle>;

}

#endif // STRUCTS_H
