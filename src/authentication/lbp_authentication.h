#ifndef LBP_AUTHENTICATION_H
#define LBP_AUTHENTICATION_H

// This
#include "feature/img_feature.h"
#include "utils/types/structs.h"

// OpenCV
#include <opencv2/ml.hpp>

namespace ProjectName {

	class InputData;
	class TrainData;

	class LBPAuthentication final
	{
	public:
		explicit LBPAuthentication();
		LBPAuthentication(const LBPAuthentication&) = delete;
		LBPAuthentication(LBPAuthentication&&) noexcept = delete;
		virtual ~LBPAuthentication() = default;

		LBPAuthentication& operator=(const LBPAuthentication&) = delete;
		LBPAuthentication& operator=(LBPAuthentication&&) noexcept = delete;

		void computeRef(const InputData& input_data, const std::string& output_folder);
		std::vector<TrainResult> computeTrain(const InputData& input_data, const std::string& output_folder) noexcept;
		void computeTest(const InputData& input_data, const std::string& output_folder, const std::vector<TrainResult>& train_results) noexcept;
		
		void initMethodParameters(const InputData& input_data); // throw runtime_error

	private:
		
		std::string algoParametersFilename(const InputData& input_data) noexcept;
		int classify(const TrainResult& train_result, const LPModelMetric& metric) noexcept;
		LPModelMetric computeDistanceMultiLPModel(const PatternFeature& pattern_feature, const cv::Mat& dct_feature, std::unique_ptr<std::ofstream>& ofs) noexcept;
		cv::Point2d computeTrainEllipseCenter(const TrainData& train_data) noexcept;
		cv::Size2d computeTrainEllipseSize(const TrainData& train_data, const TrainResult& train_result) noexcept;
		void computeTrainEllipseInfo(const TrainData& train_data, TrainResult& train_results) noexcept;
		void computeTrainMetricCentroid(const TrainData& train_data, TrainResult& train_results) noexcept;
		LPModelMetric computeMetrics(const std::string& img_path, const size_t pattern_num, std::unique_ptr<std::ofstream>& ofs) noexcept;
		void createImgModel(const InputData& input_data, int label_ref, const std::string& output_folder); // throw std::exception
		void createImgModelWith(const USetSizeT& img_indexes, const PatternFeature& pattern_feature, const VecRect& crop_rects, const int label_ref) noexcept;
		PairMatInt createNonUniformLBPMat(const LPModel& lp_model) noexcept;
		TrainResult defineClassificationEllipse(const TrainData& train_data) noexcept;
		VecTupleInt2SizeT distanceMatrix(const PatternFeature& pattern_feature, const VecRect& crop_rects) noexcept;
		bool distanceMatrixFileExists(const std::string& output_folder) noexcept;
		void findEigenvectors(const TrainData& train_data, TrainResult& train_result) noexcept;
		bool isUniformLP(const LP& lp) noexcept;
		VecTupleInt2SizeT loadDistanceMatrix(const std::string& output_ref_folder);
		TrainResult loadTrain(const std::string& output_train_folder, const std::string& train_video_name); // throw std::exception
		std::pair<PatternFeature, VecRect> modelOnRefImages(const InputData& input_data, int label_ref, const std::string& output_folder); // throw runtime_error
		TrainData prepareTrainData(const InputData& input_data, int label_ref, const std::string& output_folder) noexcept;
		void printDCTImage(const std::string& output_folder_path, const std::string& postfix_filename, const std::string& input_img_path); // throw runtime_error
		void printLBPModelImage(const std::string& output_folder_path, const std::string& postfix_filename, const std::string& input_img_path); // throw runtime_error
		void printOriginalImage(const std::string& output_folder_path, const std::string& postfix_filename, const std::string& img_filename); // throw runtime_error
		void printPredictedLabels(const std::string& output_folder, const VecInt& predicted_labels) noexcept;
		void printScatters(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept;
		void printSelectedImages(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept;
		void printStatisticReport(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept;
		void printTestResults(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept;
		cv::Mat processedDCTFeature(const cv::Mat& img, cv::Mat mask = cv::Mat()) noexcept;
		void readMethodParameters(const std::string& parameters_filename); // throw runtime_error
		USetSizeT selectImgsForModel(const VecTupleInt2SizeT& vec_dist_indexes, const std::string& output_folder, const size_t nb_imgs_ref) noexcept;
		void saveDistanceMatrix(const VecTupleInt2SizeT& indexes, const std::string& output_folder);
		void saveTrainResults(const TrainResult& train_results, const std::string& output_folder, const std::string& video_name); // throw runtime_error
		cv::Mat trainLDAFromPCAProject(const int nb_classes, const cv::Mat& metrics, const cv::Mat& labels, const cv::PCA& pca) const noexcept;
		cv::PCA trainPCA(const int nb_classes, const cv::Mat& metrics) const noexcept;
		bool trainResultFileExists(const std::string& output_train_folder, const std::string& train_video_name) noexcept;
		cv::Mat zigZagSelectNDCTCoefficients(const cv::Mat& img, int first_coeff_num, int nb_coeff) noexcept;

		// ----------------
		// MEMBER VARIABLES
		const size_t m_k_nb_imgs_for_model = 8;

		int m_dct_first_coeff;
		int m_dct_nb_coeff;
		std::string m_classification_filename;
		cv::Rect m_rect_crop_model;
		VecPairFloat m_min_max_diff;
		HoloFeature m_holo_feature;
		// ----------------
	};

} // namespace ProjectName

#endif // LBP_AUTHENTICATION_H
