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

#include "authentication/lbp_authentication.h"

// This
#include "feature/dct.h"
#include "printer/console_printer.h"
#include "printer/video_statistic_report.h"
#include "resource/input_data.h"
#include "resource/train_data.h"
#include "resource/resource_provider.h"
#include "utils/drawing/plot.h" // Include plot.h before algorithm.h because sciplot.h must be included before windows.h
#include "utils/algorithm.h"
#include "utils/operator.h"
#include "utils/types/project_variables.h"
#include "utils/types/types_opencv.h"

// C++
#include <fstream>
#include <numeric>

namespace ProjectName {

	// ----------------------------------------------------------------------------
	LBPAuthentication::LBPAuthentication()
		: m_dct_first_coeff{0}
		, m_dct_nb_coeff{1}
	{
	}

	// ----------------------------------------------------------------------------
	std::string LBPAuthentication::algoParametersFilename(const InputData& input_data) noexcept
	{
		return "../config/" + input_data.countryCode() + "/" + input_data.docType() + "/algo_parameters.yml";
	}

	// ----------------------------------------------------------------------------
	int LBPAuthentication::classify(const TrainResult& train_result, const LPModelMetric& metric) noexcept
	{
		int label = 0;
		label = predictEllipse(metric, train_result);

		return label;
	}

	// ----------------------------------------------------------------------------
	LPModelMetric LBPAuthentication::computeDistanceMultiLPModel(const PatternFeature& pattern_feature, const cv::Mat& dct_feature, std::unique_ptr<std::ofstream>& ofs) noexcept
	{
		LPModelMetric metric{FLT_MAX, 0.f};

		size_t multi_lp_model_id = 0;

		for (size_t i = 0; i < pattern_feature.size(); i++)
		{
			const LPFeature& lp_model_ref = pattern_feature[i].lpFeature();
			const cv::Mat& model_dct_feature = pattern_feature[i].dctFeature();

			cv::Rect rect_match = templateMatching(model_dct_feature, dct_feature);
			ImgFeature img_feature{dct_feature(rect_match)};

			float sum_img_dist = 0.f;

			float percent_non_uniform;
			std::tie(sum_img_dist, percent_non_uniform) = lp_model_ref.minDistanceLPModel(img_feature.lpFeature());

			if (metric.m_dist_to_multi_ref > sum_img_dist)
			{
				metric.m_percent_non_uniform = percent_non_uniform;
				metric.m_dist_to_multi_ref = sum_img_dist;
				multi_lp_model_id = i;
			}
		}

#if VERBOSE
		*(ofs.get()) << multi_lp_model_id << "\n";
#endif

		return metric;
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::computeRef(const InputData& input_data, const std::string& output_folder)
	{
		try
		{
			ConsolePrinter& console = ConsolePrinter::instance();
			m_holo_feature.resize(input_data.nbPatterns());

			const size_t nb_patterns = input_data.nbPatterns();
			for (size_t pattern_num = 1; pattern_num < nb_patterns; pattern_num++)
			{
				console.updatePatternName(std::to_string(pattern_num));

				std::string pattern_ouput_folder = output_folder + "pattern_" + std::to_string(pattern_num) + "/";
				createImgModel(input_data, static_cast<int>(pattern_num), pattern_ouput_folder);

#if VERBOSE
				const size_t nb_img_model = m_holo_feature[pattern_num].size();
				for (size_t i = 0; i < nb_img_model; i++)
				{
					cv::Mat lp_model_img = m_holo_feature[pattern_num][i].lpImage();
					std::string lp_model_img_filename = pattern_ouput_folder + "lp_model_img_" + std::to_string(i) + ".png";
					cv::imwrite(lp_model_img_filename, lp_model_img);

					cv::Mat dct_img = m_holo_feature[pattern_num][i].dctFeature();
					std::string img_dct_model_filename = pattern_ouput_folder + "img_dct_model_" + std::to_string(i) + ".png";
					cv::imwrite(img_dct_model_filename, dct_img);
				}
#endif
			}
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::computeTest(const InputData& input_data, const std::string& output_folder, const std::vector<TrainResult>& train_results) noexcept
	{
		try
		{
			ConsolePrinter& console = ConsolePrinter::instance();

			const VecString& img_test_filenames = input_data.imageFilenameList();
			size_t nb_img_test_path = img_test_filenames.size();

			const size_t nb_patterns = input_data.nbPatterns();
			for (size_t pattern_num = 1; pattern_num < nb_patterns; pattern_num++)
			{
				console.updatePatternName(std::to_string(pattern_num));

				AuthResults auth_results{nb_img_test_path};
				int64_t sum_time = 0.;

				std::unique_ptr<std::ofstream> ofs;
#if VERBOSE
				std::string pattern_ouput_folder = output_folder + "pattern_" + std::to_string(pattern_num) + "/";
				ofs = std::make_unique<std::ofstream>(pattern_ouput_folder + "_best_multi_LP_model_match.txt", std::ofstream::out);
#endif
				int count_img_treated = 0;
				int count = 0;
				for (const std::string& img_test_filename : img_test_filenames)
				{
					std::string img_test_path = input_data.imageFolder() + img_test_filename;

					size_t img_num = extractImageNumber(img_test_path);

					if (areEnoughPixelVisible(input_data.maskFolder(), img_num))
					{
						auto start = std::chrono::high_resolution_clock::now();

						LPModelMetric metric = computeMetrics(img_test_path, pattern_num, ofs);

						metric.m_dist_to_multi_ref = metric.m_dist_to_multi_ref / train_results[pattern_num].m_max_y_for_norm;
						metric.m_percent_non_uniform = metric.m_percent_non_uniform / train_results[pattern_num].m_max_x_for_norm;
						int predict_label = classify(train_results[pattern_num], metric);

						auth_results.m_lp_model_metrics[img_num] = metric;
						auth_results.m_predicted_labels[img_num] = predict_label;

						auto stop = std::chrono::high_resolution_clock::now();
						auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
						sum_time += duration.count();
						count_img_treated++;
					}
					else
					{
						auth_results.m_lp_model_metrics[img_num] = LPModelMetric{0.f, 0.f};
						auth_results.m_predicted_labels[img_num] = 0;
					}

					count++;
					console.updateProgressBar("\tLabel prediction", count, static_cast<int>(nb_img_test_path));
				}

				auth_results.m_mean_time = static_cast<double>(sum_time) / static_cast<double>(count_img_treated);
#if VERBOSE
				(ofs.get())->close();

				printTestResults(input_data, pattern_ouput_folder, auth_results, static_cast<int>(pattern_num));
#endif
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	std::vector<TrainResult> LBPAuthentication::computeTrain(const InputData& input_data, const std::string& output_folder) noexcept
	{
		std::vector<TrainResult> train_pattern_results;
		train_pattern_results.push_back(TrainResult{});

		try
		{
			const size_t nb_patterns = input_data.nbPatterns();
			for (size_t pattern_num = 1; pattern_num < nb_patterns; pattern_num++)
			{
				ConsolePrinter& console = ConsolePrinter::instance();
				console.updatePatternName(std::to_string(pattern_num));

				std::string pattern_output_folder = output_folder + "pattern_" + std::to_string(pattern_num) + "/";

				TrainResult train_results;

				if (trainResultFileExists(pattern_output_folder, input_data.videoName()))
					train_results = loadTrain(pattern_output_folder, input_data.videoName());
				else
				{
					TrainData train_data = prepareTrainData(input_data, static_cast<int>(pattern_num), pattern_output_folder);

					train_results = defineClassificationEllipse(train_data);
					saveTrainResults(train_results, pattern_output_folder, input_data.videoName());
#if VERBOSE
					Plot plot;
					plot.setPlotStyle(PlotStyle::LABELS);

					const std::string file_path = pattern_output_folder + "labels.png";
					plot.scatterAndSave(train_data.allMetrics(), input_data.imageLabels().labels(), file_path);

					float max_x = 0.f;
					float max_y = 0.f;
					const cv::Mat& all_metrics = train_data.allMetrics();
					for (int r = 0; r < all_metrics.rows; r++)
					{
						const cv::Mat& row_metric = all_metrics.row(r);
						if (row_metric.at<float>(0) > max_y)
							max_y = row_metric.at<float>(0);

						if (row_metric.at<float>(1) > max_x)
							max_x = row_metric.at<float>(1);
					}

					VecInt labels;
					cv::Mat grid_points;
					for (float x = 0; x < max_x; x += 0.01f)
						for (float y = 0; y < max_y; y += 0.01f)
						{
							cv::Mat row_point = (cv::Mat_<float>(1, 2) << y, x);
							grid_points.push_back(row_point);

							labels.push_back(classify(train_results, LPModelMetric(y, x)));
						}

					const std::string train_file_path = pattern_output_folder + "train.png";
					plot.setPlotStyle(PlotStyle::CLASSIFICATION_RESULT);
					plot.scatterAndSave(grid_points, labels, train_file_path);
#endif
				}

				train_pattern_results.push_back(train_results);
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		return train_pattern_results;
	}

	// ----------------------------------------------------------------------------
	cv::Point2d LBPAuthentication::computeTrainEllipseCenter(const TrainData& train_data) noexcept
	{
		double min_x = DBL_MAX, min_y = DBL_MAX;
		double max_x = DBL_MIN, max_y = DBL_MIN;
		std::vector<double> distances;

		const int nb_metric = train_data.metrics().rows;
		for (int i = 0; i < nb_metric; i++)
		{
			if (train_data.labels().at<int>(i) != 1)
				continue;

			double x = static_cast<double>(train_data.metrics().at<float>(i, 1));
			double y = static_cast<double>(train_data.metrics().at<float>(i, 0));

			min_x = (x < min_x) ? x : min_x;
			min_y = (y < min_y) ? y : min_y;
			max_x = (x > max_x) ? x : max_x;
			max_y = (y > max_y) ? y : max_y;
		}

		cv::Point2d ell_center;
		ell_center.x = min_x + (max_x - min_x) / 2.;
		ell_center.y = min_y + (max_y - min_y) / 2.;

		return ell_center;
	}

	// ----------------------------------------------------------------------------
	cv::Size2d LBPAuthentication::computeTrainEllipseSize(const TrainData& train_data, const TrainResult& train_result) noexcept
	{
		cv::Size2d ell_size{0., 0.};

		const int nb_metric = train_data.metrics().rows;
		for (int i = 0; i < nb_metric; i++)
		{
			if (train_data.labels().at<int>(i) != 1)
				continue;

			double x = static_cast<double>(train_data.metrics().at<float>(i, 1));
			double y = static_cast<double>(train_data.metrics().at<float>(i, 0));

			double xc = x - train_result.m_ell_center.x;
			double yc = y - train_result.m_ell_center.y;

			double xct = xc * train_result.m_ell_cos_angle + yc * train_result.m_ell_sin_angle;
			double yct = -1 * xc * train_result.m_ell_sin_angle + yc * train_result.m_ell_cos_angle;

			ell_size.width = (abs(xct * 2.) > ell_size.width) ? abs(xct * 2.) : ell_size.width;
			ell_size.height = (abs(yct * 2.) > ell_size.height) ? abs(yct * 2.) : ell_size.height;
		}

		return ell_size;
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::computeTrainEllipseInfo(const TrainData& train_data, TrainResult& train_results) noexcept
	{
		train_results.m_ell_center = computeTrainEllipseCenter(train_data);

		double angle = atan(train_results.m_eigenvectors.at<float>(1) / train_results.m_eigenvectors.at<float>(0));
		train_results.m_ell_cos_angle = cos(angle);
		train_results.m_ell_sin_angle = sin(angle);

		train_results.m_ell_size = computeTrainEllipseSize(train_data, train_results);
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::computeTrainMetricCentroid(const TrainData& train_data, TrainResult& train_results) noexcept
	{
		VecFloat metric_1;
		VecFloat metric_2;
		int nb_metrics = train_data.metrics().rows;
		for (int i = 0; i < nb_metrics; i++)
			if (train_data.labels().at<int>(i) == 1)
			{
				metric_1.push_back(train_data.metrics().at<float>(i, 0));
				metric_2.push_back(train_data.metrics().at<float>(i, 1));
			}

		float nb_good_metrics = static_cast<float>(metric_1.size());
		train_results.m_metric_centroid.first = std::reduce(metric_1.begin(), metric_1.end()) / nb_good_metrics;
		train_results.m_metric_centroid.second = std::reduce(metric_2.begin(), metric_2.end()) / nb_good_metrics;

		train_results.m_max_y_value = 0;
		for (int i = 0; i < train_data.allMetrics().rows; i++)
			train_results.m_max_y_value = std::max(train_results.m_max_y_value, train_data.allMetrics().at<float>(i, 0));
	}

	// ----------------------------------------------------------------------------
	LPModelMetric LBPAuthentication::computeMetrics(const std::string& img_path, const size_t pattern_num, std::unique_ptr<std::ofstream>& ofs) noexcept
	{
		cv::Mat img_test = cv::imread(img_path);
		cv::Mat img_gray_64f = convertUcharBGRToDoubleGray(img_test);
		cv::Mat dct_feature = processedDCTFeature(img_gray_64f);

		return computeDistanceMultiLPModel(m_holo_feature[pattern_num], dct_feature, ofs);
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::createImgModel(const InputData& input_data, int label_ref, const std::string& output_folder)
	{
		try
		{
			VecRect crop_rects;
			PatternFeature pattern_feature;
			VecTupleInt2SizeT vec_dist_indexes;

			std::tie(pattern_feature, crop_rects) = modelOnRefImages(input_data, label_ref, output_folder);

			if (distanceMatrixFileExists(output_folder))
				vec_dist_indexes = loadDistanceMatrix(output_folder);
			else
			{
				vec_dist_indexes = distanceMatrix(pattern_feature, crop_rects);
				std::sort(vec_dist_indexes.rbegin(), vec_dist_indexes.rend());

				saveDistanceMatrix(vec_dist_indexes, output_folder);
			}

			USetSizeT img_indexes = selectImgsForModel(vec_dist_indexes, output_folder, pattern_feature.size());
			createImgModelWith(img_indexes, pattern_feature, crop_rects, label_ref);
		}
		catch (std::exception e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::createImgModelWith(const USetSizeT& img_indexes, const PatternFeature& pattern_feature, const VecRect& crop_rects, const int label_ref) noexcept
	{
		for (auto it = std::begin(img_indexes); it != std::end(img_indexes); ++it)
		{
			size_t img_index = *it;

			const cv::Mat dct_feature = pattern_feature[img_index].dctFeature();
			ImgFeature to_keep{dct_feature(crop_rects[img_index])};
			m_holo_feature[static_cast<size_t>(label_ref)].push_back(to_keep);
		}
	}

	// ----------------------------------------------------------------------------
	TrainResult LBPAuthentication::defineClassificationEllipse(const TrainData& train_data) noexcept
	{
		TrainResult train_result;

		if (cv::countNonZero(train_data.labels()))
		{
			train_data.labels().copyTo(train_result.m_labels_mat);

			findEigenvectors(train_data, train_result);
			computeTrainMetricCentroid(train_data, train_result);
			computeTrainEllipseInfo(train_data, train_result);

			train_result.m_max_x_for_norm = train_data.maxXForNorm();
			train_result.m_max_y_for_norm = train_data.maxYForNorm();
		}

		return train_result;
	}

	// ----------------------------------------------------------------------------
	VecTupleInt2SizeT LBPAuthentication::distanceMatrix(const PatternFeature& pattern_feature, const VecRect& crop_rects) noexcept
	{
		ConsolePrinter& console = ConsolePrinter::instance();
		size_t nb_imgs = pattern_feature.size();
		VecTupleInt2SizeT vec_dist_indexes;

		int count = 0;
#pragma omp parallel
#pragma omp for
		for (size_t i = 0; i < nb_imgs; i++)
		{
			const ImgFeature& img_feature_i = pattern_feature[i];
			cv::Mat dct_feature_i = img_feature_i.dctFeature();
			dct_feature_i = dct_feature_i(crop_rects[i]);
			ImgFeature img_feature_ref{dct_feature_i};

			for (size_t j = (i + 1); j < nb_imgs; j++)
			{
				const ImgFeature& img_feature_j = pattern_feature[j];
				cv::Mat dct_feature_j = img_feature_j.dctFeature();
				cv::Rect rect_crop = templateMatching(dct_feature_i, dct_feature_j);
				ImgFeature img_feature{dct_feature_j(rect_crop)};

				int diff = img_feature_ref.lpFeature().distance(img_feature.lpFeature());

				vec_dist_indexes.push_back(std::make_tuple(diff, i, j));
			}

			count++;
			console.updateProgressBar("\tCompute the distance matrix", count, static_cast<int>(nb_imgs));
		}

		return vec_dist_indexes;
	}

	// ----------------------------------------------------------------------------
	bool LBPAuthentication::distanceMatrixFileExists(const std::string& output_folder) noexcept
	{
		std::string file_path = output_folder + "createImgModel.yml";

		if (std::filesystem::exists(file_path))
			return true;

		return false;
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::findEigenvectors(const TrainData& train_data, TrainResult& train_result) noexcept
	{
		int nb_classes = 2;
		cv::PCA pca = trainPCA(nb_classes, train_data.metrics());
		cv::Mat lda_eigenvectors = trainLDAFromPCAProject(nb_classes, train_data.metrics(), train_data.labels(), pca);

		cv::gemm(pca.eigenvectors, lda_eigenvectors, 1.0, cv::Mat(), 0.0, train_result.m_eigenvectors, cv::GEMM_1_T);
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::initMethodParameters(const InputData& input_data)
	{
		try
		{
			std::string algo_parameters_filename = algoParametersFilename(input_data);
			readMethodParameters(algo_parameters_filename);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	VecTupleInt2SizeT LBPAuthentication::loadDistanceMatrix(const std::string& output_ref_folder)
	{
		VecTupleInt2SizeT indexes;

		std::string input_file = output_ref_folder + "createImgModel.yml";
		cv::FileStorage fs{input_file, cv::FileStorage::READ};

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + input_file + " cannot be opened."};

		fs["indexes"] >> indexes;
		fs.release();

		ConsolePrinter& console = ConsolePrinter::instance();
		console.updateMessage("\tThe distance matrix has been loaded.");

		return indexes;
	}

	// ----------------------------------------------------------------------------
	TrainResult LBPAuthentication::loadTrain(const std::string& output_train_folder, const std::string& train_video_name)
	{
		std::string filename = output_train_folder + train_video_name + ".yml";

		TrainResult train_results;

		if (std::filesystem::exists(filename))
		{
			cv::FileStorage fs(filename, cv::FileStorage::READ);

			if (!fs.isOpened())
				throw std::runtime_error{"The file " + filename + " cannot be opened."};

			fs["features"] >> train_results.m_features;
			fs["labels_mat"] >> train_results.m_labels_mat;
			fs["eigevectors"] >> train_results.m_eigenvectors;
			fs["metric_centroid"] >> train_results.m_metric_centroid;
			fs["m_y_value"] >> train_results.m_max_y_value;
			fs["m_ell_size"] >> train_results.m_ell_size;
			fs["m_ell_cos_angle"] >> train_results.m_ell_cos_angle;
			fs["m_ell_sin_angle"] >> train_results.m_ell_sin_angle;
			fs["m_max_x_for_norm"] >> train_results.m_max_x_for_norm;
			fs["m_max_y_for_norm"] >> train_results.m_max_y_for_norm;

			fs.release();
		}
		else
			throw std::runtime_error("The filename " + filename + "doesn't exist.");

		ConsolePrinter& console = ConsolePrinter::instance();
		console.updateMessage("\tThe Train result file has been loaded.");

		return train_results;
	}

	// ----------------------------------------------------------------------------
	std::pair<PatternFeature, VecRect> LBPAuthentication::modelOnRefImages(const InputData& input_data, int label_ref, const std::string& output_folder)
	{
		VecRect crop_rects;
		PatternFeature pattern_feature;

		std::ofstream ofs(output_folder + "_equivalences.txt", std::ofstream::out);

		for (const auto& img_path : fs::directory_iterator(input_data.imageFolder()))
		{
			std::string img_filename = img_path.path().filename().string();
			std::string img_num_text = img_filename.substr(0, img_filename.find("."));
			size_t img_num = static_cast<size_t>(stoi(img_num_text));

			if (input_data.imageLabelAt(img_num) != label_ref)
				continue;

			cv::Mat img = cv::imread(img_path.path().string());
			cv::Mat gray = convertUcharBGRToDoubleGray(img);
			pattern_feature.emplace_back(processedDCTFeature(gray, input_data.cropMasks()[img_num]));
			crop_rects.push_back(input_data.cropRects()[img_num]);

			ofs << pattern_feature.size() - 1 << " = " << img_filename << "\n";
		}

		ofs.close();

		if (pattern_feature.size() == 0)
			throw std::runtime_error{"There is no image for the label " + std::to_string(label_ref) + " to construct a set of reference images."};

		return std::make_pair(pattern_feature, crop_rects);
	}

	// ----------------------------------------------------------------------------
	TrainData LBPAuthentication::prepareTrainData(const InputData& input_data, int label_ref, const std::string& output_folder) noexcept
	{
		ConsolePrinter& console = ConsolePrinter::instance();
		TrainData train_data;

		const VecString& img_filename_list = input_data.imageFilenameList();
		size_t nb_img = img_filename_list.size();

		std::unique_ptr<std::ofstream> ofs_equivalence;
		std::unique_ptr<std::ofstream> ofs_best_model;

#if VERBOSE
		ofs_equivalence = std::make_unique<std::ofstream>(output_folder + "_equivalences.txt", std::ofstream::out);
		ofs_best_model = std::make_unique<std::ofstream>(output_folder + "_best_multi_LP_model_match.txt", std::ofstream::out);
#endif

		int count = 0;
		for (const std::string& img_filename : img_filename_list)
		{
			std::string img_path = input_data.imageFolder() + img_filename;
			size_t img_num = extractImageNumber(img_path);
			int img_label = input_data.imageLabelAt(img_num);

			LPModelMetric metric = computeMetrics(img_path, static_cast<size_t>(label_ref), ofs_best_model);

			train_data.addData(img_label, label_ref, metric);

#if VERBOSE
			*(ofs_equivalence.get()) << "img num: " << img_num << " / metric index: " << train_data.metrics().rows - 1 << "\n";
#endif

			count++;
			console.updateProgressBar("\tPrepare data for training: ", count, static_cast<int>(nb_img));
		}

		train_data.normData();

#if VERBOSE
		(ofs_equivalence.get())->close();
		(ofs_best_model.get())->close();
#endif

		return train_data;
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printDCTImage(const std::string& output_folder_path, const std::string& postfix_filename, const std::string& input_img_path)
	{
		cv::Mat img = cv::imread(input_img_path);

		if (img.empty())
			throw std::runtime_error{"Error " + std::string{__func__} + ": the image " + input_img_path + " cannot be read."};

		cv::Mat img_gray_64f = convertUcharBGRToDoubleGray(img);
		cv::Mat dct_feature = processedDCTFeature(img_gray_64f);

		std::string output_file_path = output_folder_path + "dct_" + postfix_filename;
		if (!cv::imwrite(output_file_path, dct_feature))
			throw std::runtime_error{"Error " + std::string{__func__} + ": when writting the image at " + output_file_path};
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printLBPModelImage(const std::string& output_folder_path, const std::string& postfix_filename, const std::string& input_img_path)
	{
		cv::Mat img = cv::imread(input_img_path);

		if (img.empty())
			throw std::runtime_error{"Error " + std::string{__func__} + ": the image " + input_img_path + " cannot be read."};

		cv::Mat img_gray_64f = convertUcharBGRToDoubleGray(img);
		cv::Mat dct_feature = processedDCTFeature(img_gray_64f);

		ImgFeature img_feature{dct_feature};
		cv::Mat lp_img = img_feature.lpImage();

		std::string output_file_path = output_folder_path + "lp_model_" + postfix_filename;
		if (!cv::imwrite(output_file_path, lp_img))
			throw std::runtime_error{"Error " + std::string{__func__} + ": when writting the image at \"" + output_file_path + "\"."};
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printOriginalImage(const std::string& output_folder_path, const std::string& postfix_filename, const std::string& input_img_path)
	{
		cv::Mat img = cv::imread(input_img_path);

		if (img.empty())
			throw std::runtime_error{"Error " + std::string{__func__} + ": the image \"" + input_img_path + "\" cannot be read."};

		std::string output_file_path = output_folder_path + "img_" + postfix_filename;
		if (!cv::imwrite(output_file_path, img))
			throw std::runtime_error{"Error " + std::string{__func__} + ": when writting the image at \"" + output_file_path + "\"."};
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printPredictedLabels(const std::string& output_folder, const VecInt& predicted_labels) noexcept
	{
		ConsolePrinter& console = ConsolePrinter::instance();
		console.updateMessage("\tPrint results: predicted labels");

		std::string output_file = output_folder + "predicted_labels.yml";
		cv::FileStorage fs{output_file, cv::FileStorage::WRITE};

		if (!fs.isOpened())
		{
			std::cerr << "The file " << output_file << " cannot be opened." << std::endl;
			return;
		}

		fs << "predicted_labels" << predicted_labels;

		fs.release();
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printScatters(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept
	{
		try
		{
			ConsolePrinter& console = ConsolePrinter::instance();
			console.updateMessage("\tPrint results: scatters");

			Plot plot;
			std::string output_img_path;

			plot.setPlotStyle(PlotStyle::CLASSIFICATION_RESULT);
			output_img_path = output_folder + "_scatter.png";
			plot.scatterAndSave(auth_results.m_lp_model_metrics, auth_results.m_predicted_labels, output_img_path);

			plot.setPlotStyle(PlotStyle::LABELS);
			output_img_path = output_folder + "_labels_test_" + std::to_string(pattern_num) + ".png";
			plot.scatterAndSave(auth_results.m_lp_model_metrics, input_data.labelsTestAt(static_cast<size_t>(pattern_num)), output_img_path);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printSelectedImages(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept
	{
		try
		{
			ConsolePrinter& console = ConsolePrinter::instance();
			console.updateMessage("\tPrint results: selected images");

			const VecString& img_test_filenames = input_data.imageFilenameList();
			for (const std::string& img_filename : img_test_filenames)
			{
				size_t img_num = extractImageNumber(img_filename);

				if (auth_results.m_predicted_labels[img_num] == 1)
				{
					int img_label = input_data.labelsTestAt(pattern_num)[img_num];
					std::string postfix_filename = std::to_string(img_label) + "_" + std::to_string(img_num) + ".png";

					std::string input_img_path = input_data.imageFolder() + img_filename;
					printOriginalImage(output_folder, postfix_filename, input_img_path);
					printDCTImage(output_folder, postfix_filename, input_img_path);
					printLBPModelImage(output_folder, postfix_filename, input_img_path);
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printStatisticReport(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept
	{
		try
		{
			ConsolePrinter& console = ConsolePrinter::instance();
			console.updateMessage("\tPrint results: output report");

			VideoStatisticReport video_statistic_report;
			video_statistic_report.write(output_folder, input_data, auth_results, pattern_num);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::printTestResults(const InputData& input_data, const std::string& output_folder, const AuthResults& auth_results, int pattern_num) noexcept
	{
		printSelectedImages(input_data, output_folder, auth_results, pattern_num);
		printScatters(input_data, output_folder, auth_results, pattern_num);
		printStatisticReport(input_data, output_folder, auth_results, pattern_num);
		printPredictedLabels(output_folder, auth_results.m_predicted_labels);
	}

	// ----------------------------------------------------------------------------
	cv::Mat LBPAuthentication::processedDCTFeature(const cv::Mat& img, cv::Mat mask) noexcept
	{
		cv::Mat img_blurred;
		cv::GaussianBlur(img, img_blurred, cv::Size{5, 5}, 0);
		cv::Mat dct_feature = zigZagSelectNDCTCoefficients(img_blurred, m_dct_first_coeff, m_dct_nb_coeff);
		dct_feature = differenceOfGaussians(dct_feature, cv::Size{1, 1}, cv::Size{3, 3});
		cv::normalize(dct_feature, dct_feature, 255.0, 0.0, cv::NORM_MINMAX);

		cv::Mat dct_feature_masked;
		if (mask.empty())
			dct_feature.copyTo(dct_feature_masked);
		else
		{
			if (mask.rows % 2 == 1 || mask.cols % 2 == 1)
			{
				int top = 0;
				int left = 0;
				int bottom = mask.rows % 2;
				int right = mask.cols % 2;

				cv::Mat mask_tmp = mask.clone();
				cv::copyMakeBorder(mask_tmp, mask, top, bottom, left, right, cv::BORDER_REFLECT101);
			}

			dct_feature.copyTo(dct_feature_masked, mask);
		}

		return dct_feature_masked;
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::readMethodParameters(const std::string& parameters_filename)
	{
		cv::FileStorage fs(parameters_filename, cv::FileStorage::READ);

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + parameters_filename + " cannot be opened."};

		fs["dct_first_coeff"] >> m_dct_first_coeff;
		fs["dct_nb_coeff"] >> m_dct_nb_coeff;

		fs.release();
	}

	// ----------------------------------------------------------------------------
	USetSizeT LBPAuthentication::selectImgsForModel(const VecTupleInt2SizeT& vec_dist_indexes, const std::string& output_folder, const size_t nb_imgs_ref) noexcept
	{
		USetSizeT img_indexes;

		size_t nb_imgs_for_model = std::min(m_k_nb_imgs_for_model, nb_imgs_ref);
		for (int i = 0; img_indexes.size() < nb_imgs_for_model; i++)
		{
			TupleInt2SizeT index = vec_dist_indexes[i];
			size_t imgs_ref_index_i = std::get<1>(index);
			size_t imgs_ref_index_j = std::get<2>(index);

			img_indexes.insert(imgs_ref_index_i);
			img_indexes.insert(imgs_ref_index_j);
		}

#if VERBOSE
		std::ofstream ofs(output_folder + "_index_selected_ref_images.txt", std::ofstream::out);
		for (auto it = std::begin(img_indexes); it != std::end(img_indexes); ++it)
			ofs << (*it) << "\n";
		ofs.close();
#endif

		return img_indexes;
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::saveDistanceMatrix(const VecTupleInt2SizeT& indexes, const std::string& output_folder)
	{
		std::string output_file = output_folder + "createImgModel.yml";
		cv::FileStorage fs{output_file, cv::FileStorage::WRITE};

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + output_file + " cannot be opened."};

		fs << "indexes" << indexes;

		fs.release();
	}

	// ----------------------------------------------------------------------------
	void LBPAuthentication::saveTrainResults(const TrainResult& train_results, const std::string& output_folder, const std::string& video_name)
	{
		std::string classification_path = output_folder + video_name + ".yml";
		cv::FileStorage fs(classification_path, cv::FileStorage::WRITE);

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + classification_path + " cannot be opened."};

		fs << "features" << train_results.m_features;
		fs << "labels_mat" << train_results.m_labels_mat;
		fs << "eigevectors" << train_results.m_eigenvectors;
		fs << "metric_centroid" << train_results.m_metric_centroid;
		fs << "m_y_value" << train_results.m_max_y_value;
		fs << "m_ell_size" << train_results.m_ell_size;
		fs << "m_ell_cos_angle" << train_results.m_ell_cos_angle;
		fs << "m_ell_sin_angle" << train_results.m_ell_sin_angle;
		fs << "m_max_x_for_norm" << train_results.m_max_x_for_norm;
		fs << "m_max_y_for_norm" << train_results.m_max_y_for_norm;

		fs.release();
	}

	// ----------------------------------------------------------------------------
	cv::Mat LBPAuthentication::trainLDAFromPCAProject(const int nb_classes, const cv::Mat& metrics, const cv::Mat& labels, const cv::PCA& pca) const noexcept
	{
		int num_comp = nb_classes - 1;
		cv::LDA lda{pca.project(metrics), labels, num_comp};

		cv::Mat lda_eigenvectors;
		lda.eigenvectors().convertTo(lda_eigenvectors, pca.eigenvectors.type());

		return lda_eigenvectors;
	}

	// ----------------------------------------------------------------------------
	cv::PCA LBPAuthentication::trainPCA(const int nb_classes, const cv::Mat& metrics) const noexcept
	{
		cv::PCA pca{metrics, cv::Mat(), cv::PCA::DATA_AS_ROW, (metrics.rows - nb_classes)};
		return pca;
	}

	// ----------------------------------------------------------------------------
	bool LBPAuthentication::trainResultFileExists(const std::string& output_train_folder, const std::string& train_video_name) noexcept
	{
		std::string filename = output_train_folder + train_video_name + ".yml";

		if (std::filesystem::exists(filename))
			return true;

		return false;
	}

	// ----------------------------------------------------------------------------
	cv::Mat LBPAuthentication::zigZagSelectNDCTCoefficients(const cv::Mat& img, int first_coeff_num, int nb_coeff) noexcept
	{
		DCT dct;
		cv::Mat dct_coeffs = dct.computeDCTCoeff(img);
		cv::Mat n_coeffs = dct.selectNZigZagCoefficients(dct_coeffs, first_coeff_num, nb_coeff);
		cv::Mat img_n_coeffs = dct.computeIDCTCoeff(n_coeffs);

		return img_n_coeffs;
	}

} // namespace ProjectName
