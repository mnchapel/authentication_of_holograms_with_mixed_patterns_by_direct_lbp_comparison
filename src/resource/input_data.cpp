#include "resource/input_data.h"

// This
#include "utils/algorithm.h"
#include "utils/operator.h"

// C++
#include <filesystem>
#include <iostream>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace fs = std::filesystem;

namespace ProjectName {

	// ----------------------------------------------------------------------------
	InputData::InputData(const std::string& ovd_metadata_path)
		: m_area_num{0}
	{
		try
		{
			readInputFile();
			readOVDMetadataFile(ovd_metadata_path);
			readVideoMetadataFile();
			readFilenamesFile();
			readHologramInfo();
			readLabelsFile();
			readCropMasksFile();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void InputData::readCropMasksFile()
	{
		if (fs::exists(m_crop_masks_file) && fs::is_regular_file(m_crop_masks_file))
		{
			cv::FileStorage fs(m_crop_masks_file, cv::FileStorage::READ);

			if (!fs.isOpened())
				throw std::runtime_error{"The file " + m_crop_masks_file + " cannot be opened."};

			std::string img_filename = m_img_folder + m_filename_list.front();
			cv::Size img_size = cv::imread(img_filename).size();

			cv::FileNode fn = fs["crop_masks"];
			for (cv::FileNodeIterator it = fn.begin(); it != fn.end(); it++)
			{
				cv::Point center;
				int radius;
				cv::FileNode item = *it;
				item["center_x"] >> center.x;
				item["center_y"] >> center.y;
				item["radius"] >> radius;

				if (radius > 0)
				{
					m_crop_masks.emplace_back(img_size, CV_8UC1, cv::Scalar{0});
					cv::circle(m_crop_masks.back(), center, radius, cv::Scalar{255}, -1);

					cv::Point upper_left{center.x - radius - 1, center.y - radius - 1};
					cv::Point lower_right{center.x + radius - 1, center.y + radius - 1};

					upper_left = clampToImgSize(img_size, upper_left);
					lower_right = clampToImgSize(img_size, lower_right);

					m_crop_rects.emplace_back(upper_left, lower_right);
				}
				else
				{
					m_crop_masks.emplace_back(img_size, CV_8UC1, cv::Scalar{255});
					m_crop_rects.emplace_back(0, 0, img_size.width, img_size.height);
				}
			}

			fs.release();
		}
		else
		{
			const std::string img_filename = m_img_folder + m_filename_list.front();
			const size_t nb_imgs = m_filename_list.size();
			const cv::Size img_size = cv::imread(img_filename).size();

			cv::Mat mask{img_size, CV_8UC1, cv::Scalar{255}};
			m_crop_masks = VecMat{nb_imgs, mask};
		}
	}

	// ----------------------------------------------------------------------------
	void InputData::readFilenamesFile()
	{
		cv::FileStorage fs(m_filenames_file, cv::FileStorage::READ);

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + m_filenames_file + " cannot be opened."};

		cv::read(fs["filenames"], m_filename_list);

		fs.release();
	}

	// ----------------------------------------------------------------------------
	void InputData::readHologramInfo()
	{
		std::string file_path = "../config/" + m_country_code + "/" + m_doc_type + "/hologram_info.yml";
		cv::FileStorage fs(file_path, cv::FileStorage::READ);

		if (!fs.isOpened())
			throw std::runtime_error{ "The file " + file_path + " cannot be opened." };

		std::string key = "nb_labels_" + m_area_num;
		fs[key] >> m_nb_patterns;

		fs.release();
	}

	// ----------------------------------------------------------------------------
	void InputData::readInputFile()
	{
		std::string input_file_path = "../config/input.yml";

		cv::FileStorage fs(input_file_path, cv::FileStorage::READ);

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + input_file_path + " cannot be opened."};

		cv::read(fs["data_folder"], m_data_folder, "");

		fs.release();
	}

	// ----------------------------------------------------------------------------
	void InputData::readLabelsFile()
	{
		try
		{
			m_image_labels = std::make_unique<ImageLabels>(*this);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void InputData::readVideoMetadataFile()
	{
		cv::FileStorage fs(m_metadata_file, cv::FileStorage::READ);

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + m_metadata_file + " cannot be opened."};

		int doc_id = 0;

		cv::read(fs["id_doc_country"], m_country_code, "");
		cv::read(fs["id_doc_id"], doc_id, 0);
		cv::read(fs["id_doc_label"], m_doc_label, "");
		cv::read(fs["id_doc_type"], m_doc_type, "");
		cv::read(fs["camera_type"], m_smartphone, "");

		m_doc_id = std::to_string(doc_id);

		fs.release();
	}

	// ----------------------------------------------------------------------------
	void InputData::readOVDMetadataFile(const std::string& file_path)
	{
		cv::FileStorage fs(file_path, cv::FileStorage::READ);

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + file_path + " cannot be opened."};

		cv::read(fs["area_num"], m_area_num, "");
		cv::read(fs["crop_masks_path"], m_crop_masks_file, "");
		cv::read(fs["filenames_file_path"], m_filenames_file, "");
		cv::read(fs["img_folder_path"], m_img_folder, "");
		cv::read(fs["labels_file_path"], m_labels_file, "");
		cv::read(fs["labels_test_file_path"], m_labels_test_file_path);
		cv::read(fs["mask_folder_path"], m_mask_folder, "");
		cv::read(fs["metadata_file_path"], m_metadata_file, "");
		cv::read(fs["video_name"], m_video_name, "");

		m_crop_masks_file = m_data_folder + m_crop_masks_file;
		m_filenames_file = m_data_folder + m_filenames_file;
		m_img_folder = m_data_folder + m_img_folder;
		m_labels_file = m_data_folder + m_labels_file;
		m_metadata_file = m_data_folder + m_metadata_file;
		m_mask_folder = m_data_folder + m_mask_folder;

		fs.release();
	}

} // namespace ProjectName
