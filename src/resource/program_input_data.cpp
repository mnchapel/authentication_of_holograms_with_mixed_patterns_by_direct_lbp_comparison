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

#include "resource/program_input_data.h"

// C++
#include <stdexcept>

// OpenCV
#include <opencv2/core/persistence.hpp>


namespace ProjectName
{

	// ----------------------------------------------------------------------------
	ProgramInputData::ProgramInputData()
	{
        std::string input_file_path = "../config/input.yml";

        cv::FileStorage fs(input_file_path, cv::FileStorage::READ);

        if (!fs.isOpened())
            throw std::runtime_error{ "The file " + input_file_path + " cannot be opened." };

        cv::read(fs["ref_ovd"], m_ref_ovd, "");
        cv::read(fs["train_ovd"], m_train_ovd);
        cv::read(fs["test_ovd"], m_test_ovd);

        fs.release();
	}

} // namespace ProjectName
