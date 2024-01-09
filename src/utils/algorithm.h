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

#ifndef ALGORITHM_H
#define ALGORITHM_H

// This
#include "resource/input_data.h"
#include "utils/types/structs.h"

// C++
#include <filesystem>
#include <fstream>
#include <iostream>

#if defined (_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace fs = std::filesystem;

namespace ProjectName {

	// ----------------------------------------------------------------------------
	inline bool areEnoughPixelVisible(const std::string& mask_folder_path, size_t img_num) noexcept
	{
		std::string mask_filename = mask_folder_path + std::to_string(img_num) + ".png";
		cv::Mat mask = cv::imread(mask_filename, cv::IMREAD_GRAYSCALE);
		float nb_not_black_pixel = static_cast<float>(cv::countNonZero(mask));

		float nb_img_pixel = static_cast<float>(mask.rows * mask.cols);
		float percent_valid = 0.9f;
		if (nb_not_black_pixel > (percent_valid * nb_img_pixel))
			return true;

		return false;
	}

	// ----------------------------------------------------------------------------
	inline cv::Point clampToImgSize(const cv::Size img_size, const cv::Point& p) noexcept
	{
		cv::Point p_clamped;

		p_clamped.x = std::max(p.x, 0);
		p_clamped.x = std::min(p.x, img_size.width);

		p_clamped.y = std::max(p.y, 0);
		p_clamped.y = std::min(p.y, img_size.height);

		return p_clamped;
	}

	// ----------------------------------------------------------------------------
	inline double computeMean(const VecDouble& values) noexcept
	{
		double sum = 0.;

		size_t nb_value = values.size();
		for (size_t i = 0; i < nb_value; i++)
			sum += values[i];

		return sum / static_cast<double>(nb_value);
	}

	// ----------------------------------------------------------------------------
	inline cv::Mat convertUcharBGRToDoubleGray(const cv::Mat& img) noexcept
	{
		cv::Mat img_gray;
		cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

		cv::Mat img_64f;
		img_gray.convertTo(img_64f, CV_64F, 1.0 / 255.0);

		return img_64f;
	}

	// ----------------------------------------------------------------------------
	inline void createFolder(const std::string& folder_path)
	{
		std::string folder_path_2 = folder_path;
		if (folder_path[folder_path.size() - 1] == '/')
			folder_path_2 = folder_path.substr(0, folder_path.size() - 1);

		if (!fs::exists(folder_path_2))
		{
			if (!fs::create_directories(folder_path_2))
				throw std::runtime_error{"The output folder " + folder_path_2 + "cannot be created."};
		}
	}

	// ----------------------------------------------------------------------------
	inline cv::Mat differenceOfGaussians(const cv::Mat& img, cv::Size size_1, cv::Size size_2) noexcept
	{
		cv::Mat g1, g2;
		cv::GaussianBlur(img, g1, size_1, 0);
		cv::GaussianBlur(img, g2, size_2, 0);

		return g1 - g2;
	}

	// ----------------------------------------------------------------------------
	inline size_t extractImageNumber(const std::string& img_path) noexcept
	{
		std::string img_filename = fs::path{img_path}.filename().string();
		std::string img_num_text = img_filename.substr(0, img_filename.find("."));

		return static_cast<size_t>(stoi(img_num_text));
	}

	// ----------------------------------------------------------------------------
	inline bool isPointOutOfImageBounds(const cv::Mat& img, int row, int col) noexcept
	{
		if (row < 0 || row >= img.rows || col < 0 || col >= img.cols)
			return true;

		return false;
	}

	// ----------------------------------------------------------------------------
	template<typename T>
	inline bool isPointOutOfModel(const T& model, size_t row, size_t col) noexcept
	{
		size_t nb_rows = model.size();
		size_t nb_cols = model.front().size();

		if (row < 0 || row >= nb_rows || col < 0 || col >= nb_cols)
			return true;

		return false;
	}

	// ----------------------------------------------------------------------------
	inline int predictEllipse(const LPModelMetric& metric_test, const TrainResult& train_result) noexcept
	{
		int label = 0;

		double x = static_cast<double>(metric_test.m_percent_non_uniform);
		double y = static_cast<double>(metric_test.m_dist_to_multi_ref);

		cv::Point2d ell_center = train_result.m_ell_center;
		double cos_angle = train_result.m_ell_cos_angle;
		double sin_angle = train_result.m_ell_sin_angle;

		double xc = x - ell_center.x;
		double yc = y - ell_center.y;

		double xct = xc * cos_angle + yc * sin_angle;
		double yct = -1 * xc * sin_angle + yc * cos_angle;

		double ell_semi_width = train_result.m_ell_size.width / 2.;
		double ell_semi_height = train_result.m_ell_size.height / 2.;

		double dist = (pow(xct, 2) / pow(ell_semi_width, 2)) + (pow(yct, 2) / pow(ell_semi_height, 2));

		if (dist <= 1)
			label = 1;

		return label;
	}

	// ----------------------------------------------------------------------------
	inline void setCursorVisibility(bool visible) noexcept
	{
#if defined(_WIN32)
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = visible;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
#elif defined(__linux__)
		cout << (visible ? "\033[?25h" : "\033[?25l");
#endif
	}

	// ----------------------------------------------------------------------------
	inline cv::Rect templateMatching(const cv::Mat& img_ref, const cv::Mat& img) noexcept
	{
		cv::Mat match_template_res;
		int method = cv::TM_CCORR;
		cv::matchTemplate(img, img_ref, match_template_res, method);
		double min_val, max_val;
		cv::Point min_loc, max_loc;
		cv::minMaxLoc(match_template_res, &min_val, &max_val, &min_loc, &max_loc);

		cv::Point top_left;
		if (method == cv::TM_SQDIFF || method == cv::TM_SQDIFF_NORMED)
			top_left = min_loc;
		else
			top_left = max_loc;

		cv::Point bottom_right(top_left.x + img_ref.cols, top_left.y + img_ref.rows);
		cv::Rect match_rect(top_left, bottom_right);

		return match_rect;
	}

} // namespace ProjectName

#endif // ALGORITHM_H
