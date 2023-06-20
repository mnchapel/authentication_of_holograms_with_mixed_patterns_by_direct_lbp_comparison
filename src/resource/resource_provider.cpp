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
