#include "resource/train_data.h"

// This
#include "utils/types/structs.h"


namespace ProjectName
{

	// ----------------------------------------------------------------------------
	TrainData::TrainData()
	{
		m_all_labels = cv::Mat(0, 1, CV_32SC1);
		m_labels = cv::Mat(0, 1, CV_32SC1);
		
		m_all_metrics = cv::Mat(0, 2, CV_32FC1);
		m_metrics = cv::Mat(0, 2, CV_32FC1);
	}

	// ----------------------------------------------------------------------------
	void TrainData::addData(const int img_label, const int label_ref, const LPModelMetric& metric) noexcept
	{
		int label = 0;
		if (img_label == label_ref)
			label = 1;

		cv::Mat metric_row = (cv::Mat_<float>(1, 2) << metric.m_dist_to_multi_ref, metric.m_percent_non_uniform);
		cv::Mat label_row = (cv::Mat_<int>(1, 1) << label);

		m_all_metrics.push_back(metric_row);
		m_all_labels.push_back(label_row);

		if (img_label != 0)
		{
			m_metrics.push_back(metric_row);
			m_labels.push_back(label_row);
		}

		m_max_x_for_norm = (metric.m_percent_non_uniform > m_max_x_for_norm) ? metric.m_percent_non_uniform : m_max_x_for_norm;
		m_max_y_for_norm = (metric.m_dist_to_multi_ref > m_max_y_for_norm) ? metric.m_dist_to_multi_ref : m_max_y_for_norm;
	}

	// ----------------------------------------------------------------------------
	void TrainData::normData() noexcept
	{
		normData(m_all_metrics, m_max_x_for_norm, m_max_y_for_norm);
		normData(m_metrics, m_max_x_for_norm, m_max_y_for_norm);
	}

	// ----------------------------------------------------------------------------
	void TrainData::normData(cv::Mat& data, double max_x, double max_y) noexcept
	{
		const int nb_data = data.rows;
		for(int i = 0; i < nb_data; i++)
		{
			data.at<float>(i, 0) = data.at<float>(i, 0) / max_y;
			data.at<float>(i, 1) = data.at<float>(i, 1) / max_x;
		}
	}

} // namespace ProjectName
