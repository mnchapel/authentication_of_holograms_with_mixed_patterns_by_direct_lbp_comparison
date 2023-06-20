#include "printer/video_statistic_report.h"

// This
#include "resource/input_data.h"

// C++
#include <assert.h>
#include <fstream>

// OpenCV
#include <opencv2/core/persistence.hpp>


namespace ProjectName
{

    // ----------------------------------------------------------------------------
    VideoStatisticReport::PredictiveValue VideoStatisticReport::computePredictiveValue(const VecInt& labels, const VecInt& predicted_labels, const int label_ref) noexcept
    {
        assert(labels.size() == predicted_labels.size());

        PredictiveValue pv;

        size_t nb_elems = labels.size();
        for (size_t ind = 0; ind < nb_elems; ind++)
        {
            if (labels[ind] == label_ref)
            {
                if (predicted_labels[ind] == 1)
                    pv.m_nb_tp++;
                else
                    pv.m_nb_fn++;
            }   
            else
            {
                if (predicted_labels[ind] == 1)
                    pv.m_nb_fp++;
                else
                    pv.m_nb_tn++;
            }
        }

        return pv;
    }

    // ----------------------------------------------------------------------------
    VideoStatisticReport::ResultStatistics VideoStatisticReport::computeResultStatistics(const PredictiveValue& pv) noexcept
    {
        ResultStatistics rs;

        // Precision
        int p = pv.m_nb_tp + pv.m_nb_fp;
        if(p != 0)
            rs.m_precision = static_cast<float>(pv.m_nb_tp) / static_cast<float>(pv.m_nb_tp + pv.m_nb_fp);

        // Recall
        int r = pv.m_nb_tp + pv.m_nb_fn;
        if(r != 0)
            rs.m_recall = static_cast<float>(pv.m_nb_tp) / static_cast<float>(pv.m_nb_tp + pv.m_nb_fn);

        return rs;
    }

    // ----------------------------------------------------------------------------
    void VideoStatisticReport::write(const std::string& output_folder, const InputData& input_data, const AuthResults& auth_results, const int label_ref)
    {
		const VecInt& labels = input_data.labelsTestAt(static_cast<size_t>(label_ref));
		const VecInt& predicted_labels = auth_results.m_predicted_labels;

        assert(labels.size() == predicted_labels.size());

        PredictiveValue pv = computePredictiveValue(labels, predicted_labels, label_ref);
        ResultStatistics rs = computeResultStatistics(pv);

		std::string file_path = output_folder + "output_report.yml";
		cv::FileStorage fs{file_path, cv::FileStorage::WRITE};

		if(!fs.isOpened())
			throw std::runtime_error{"The file " + file_path + " cannot be opened."};

        fs << "precision" << rs.m_precision;
        fs << "recall" << rs.m_recall;
		fs << "nb_img" << static_cast<int>(labels.size());
		fs << "tp" << pv.m_nb_tp;
		fs << "tn" << pv.m_nb_tn;
		fs << "fp" << pv.m_nb_fp;
		fs << "fn" << pv.m_nb_fn;
		fs << "mean_time" << auth_results.m_mean_time;
		fs << "camera_type" << input_data.smartphone();
		fs << "doc_id" << input_data.docId();
		fs << "doc_label" << input_data.docLabel();
		fs << "pattern_num" << std::to_string(label_ref);
		fs << "video_name" << input_data.videoName();

		fs.release();
    }

} // namespace ProjectName
