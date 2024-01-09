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

#include "resource/resource_provider.h"

// C++
#include <iostream>


namespace ProjectName
{
	// ----------------------------------------------------------------------------
	ResourceProvider& ResourceProvider::instance() noexcept
	{
		static ResourceProvider m_instance;
		return m_instance;
	}

	// ----------------------------------------------------------------------------
    const ProgramInputData& ResourceProvider::requestProgramInputData() const noexcept
    {
        return m_program_input_data;
    }

    // ----------------------------------------------------------------------------
    InputData& ResourceProvider::requestRefOVD(const std::string& metadata_path) noexcept
    {
        m_ref = std::make_unique<InputData>(metadata_path);
        m_result_folder_paths.updateRef(*(m_ref.get()));
        return *(m_ref.get());
    }

    // ----------------------------------------------------------------------------
    InputData& ResourceProvider::requestTestOVD(const std::string& metadata_path)
    {
		try
		{
			m_test = std::make_unique<InputData>(metadata_path);
			m_result_folder_paths.updateTest(*(m_test.get()));
			return *(m_test.get());
		}
		catch(const std::exception& e)
		{
			throw e;
		}
    }

    // ----------------------------------------------------------------------------
    InputData& ResourceProvider::requestTrainOVD(const std::string& metadata_path) noexcept
    {
        m_train = std::make_unique<InputData>(metadata_path);
        m_result_folder_paths.updateTrain(*(m_train.get()));
        return *(m_train.get());
    }

    // ----------------------------------------------------------------------------
    std::string ResourceProvider::requestResultFolderRef() const noexcept
    {
        return m_result_folder_paths.resultFolderRefArea();
    }

    // ----------------------------------------------------------------------------
    std::string ResourceProvider::requestResultFolderTest() const noexcept
    {
        return m_result_folder_paths.resultFolderTestArea();
    }

    // ----------------------------------------------------------------------------
    std::string ResourceProvider::requestResultFolderTrain() const noexcept
    {
        return m_result_folder_paths.resultFolderTrainArea();
    }

} // namespace ProjectName
