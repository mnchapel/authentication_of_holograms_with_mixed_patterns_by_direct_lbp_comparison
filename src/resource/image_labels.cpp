#include "resource/image_labels.h"

// This
#include "resource/input_data.h"
#include "utils/operator.h"

// C++
#include <filesystem>
#include <iostream>

// OpenCV
#include <opencv2/core.hpp>

namespace fs = std::filesystem;

namespace ProjectName {

	// ----------------------------------------------------------------------------
	ImageLabels::ImageLabels(InputData& input_data)
	{
		try
		{
			m_labels_test.resize(input_data.nbPatterns());

			m_labels_ref = load(input_data.labelsFile());

			for (size_t i = 0; i < input_data.nbPatterns() - 1; i++)
				m_labels_test[i] = load(input_data.labelsTestFilePath(i));
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	VecInt ImageLabels::load(const std::string& path)
	{
		cv::FileStorage fs(path, cv::FileStorage::READ);

		if (!fs.isOpened())
		{
			std::cout << "The file " << path << " cannot be opened." << std::endl;
			throw std::runtime_error{"The file " + path + " cannot be opened."};
		}

		VecInt labels;
		cv::read(fs["img_labels"], labels);

		fs.release();

		return labels;
	}

} // namespace ProjectName
