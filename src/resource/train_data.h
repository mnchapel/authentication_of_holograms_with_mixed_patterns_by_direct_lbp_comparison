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
