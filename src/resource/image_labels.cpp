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
