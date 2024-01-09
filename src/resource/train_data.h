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

#ifndef TRAIN_DATA_H
#define TRAIN_DATA_H

#include "utils/types/types_opencv.h"

namespace ProjectName {

	struct LPModelMetric;

	class TrainData final
	{
	public:
		explicit TrainData();
		TrainData(const TrainData&) = default;
		TrainData(TrainData&&) noexcept = default;
		virtual ~TrainData() = default;

		TrainData& operator=(const TrainData&) = default;
		TrainData& operator=(TrainData&&) noexcept = default;

		// -------
		// GETTERS
		inline double maxXForNorm() const noexcept { return m_max_x_for_norm; }
		inline double maxYForNorm() const noexcept { return m_max_y_for_norm; }
		inline const cv::Mat& allMetrics() const noexcept { return m_all_metrics; }
		inline const cv::Mat& metrics() const noexcept { return m_metrics; }
		inline const cv::Mat& labels() const noexcept { return m_labels; }
		// -------

		void addData(const int img_label, const int label_ref, const LPModelMetric& metric) noexcept;
		void normData() noexcept;

	private:
		void normData(cv::Mat& data, double max_x, double max_y) noexcept;

		// ----------------
		// MEMBER VARIABLES
		double m_max_x_for_norm;
		double m_max_y_for_norm;
		cv::Mat m_all_labels;
		cv::Mat m_labels;
		cv::Mat m_metrics;
		cv::Mat m_all_metrics;
		VecScalar m_label_colors;
		VecScalar m_all_label_colors;
		// ----------------
	};

} // namespace ProjectName

#endif // TRAIN_DATA_H
