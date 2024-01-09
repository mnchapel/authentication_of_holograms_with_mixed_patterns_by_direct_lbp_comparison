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

#include "resource/result_folder_paths.h"

// This
#include "resource/input_data.h"
#include "resource/resource_provider.h"
#include "utils/algorithm.h"

// C++
#include <filesystem>

namespace fs = std::filesystem;

namespace ProjectName {

	// ----------------------------------------------------------------------------
	void ResultFolderPaths::updatePatterns(const std::string& folder_path, const size_t nb_patterns) noexcept
	{
		for (size_t pattern_num = 1; pattern_num < nb_patterns; pattern_num++)
		{
			std::string pattern_folder_path = folder_path + "pattern_" + std::to_string(pattern_num) + "/";
			createFolder(pattern_folder_path);
		}
	}

	// ----------------------------------------------------------------------------
	void ResultFolderPaths::updateRef(const InputData& input_data) noexcept
	{
		const ProgramInputData& program_input_data = ResourceProvider::instance().requestProgramInputData();

		m_ref = "../results/" + input_data.countryCode() + "/" + input_data.docType() + "/ref_" + input_data.docLabel() + "_" + input_data.videoName() + "/";

		createFolder(m_ref);

		m_ref_area = m_ref + "ref_result/";
		createFolder(m_ref_area);

		updatePatterns(m_ref_area, input_data.nbPatterns());
	}

	// ----------------------------------------------------------------------------
	void ResultFolderPaths::updateTest(const InputData& input_data) noexcept
	{
		m_test = "test_" + input_data.docLabel() + "_" + input_data.videoName() + "/";
		createFolder(m_ref + m_train + m_test);

		m_test_area = m_test;
		createFolder(m_ref + m_train + m_test_area);

		updatePatterns(m_ref + m_train + m_test_area, input_data.nbPatterns());
	}

	// ----------------------------------------------------------------------------
	void ResultFolderPaths::updateTrain(const InputData& input_data) noexcept
	{
		m_train = "train_" + input_data.docLabel() + "_" + input_data.videoName() + "/";
		createFolder(m_ref + m_train);

		m_train_area = m_train + "train_result/";
		createFolder(m_ref + m_train_area);

		updatePatterns(m_ref + m_train_area, input_data.nbPatterns());
	}

} // namespace ProjectName
