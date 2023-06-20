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
