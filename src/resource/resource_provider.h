#ifndef RESOURCE_PROVIDER_H
#define RESOURCE_PROVIDER_H

// This
#include "resource/input_data.h"
#include "resource/program_input_data.h"
#include "resource/result_folder_paths.h"

namespace ProjectName {
	class ResourceProvider
	{
	public:
		static ResourceProvider& instance() noexcept;

		const ProgramInputData& requestProgramInputData() const noexcept;
		InputData& requestRefOVD(const std::string& metadata_path) noexcept;
		InputData& requestTestOVD(const std::string& metadata_path);
		InputData& requestTrainOVD(const std::string& metadata_path) noexcept;
		std::string requestResultFolderRef() const noexcept;
		std::string requestResultFolderTest() const noexcept;
		std::string requestResultFolderTrain() const noexcept;

	private:
		explicit ResourceProvider() = default;
		ResourceProvider(const ResourceProvider&) = delete;
		ResourceProvider(ResourceProvider&&) noexcept = default;
		virtual ~ResourceProvider() = default;

		ResourceProvider& operator=(const ResourceProvider&) = delete;
		ResourceProvider& operator=(ResourceProvider&&) noexcept = default;

		// ----------------
		// MEMBER VARIABLES
		std::unique_ptr<InputData> m_ref;
		std::unique_ptr<InputData> m_test;
		std::unique_ptr<InputData> m_train;
		ProgramInputData m_program_input_data;
		ResultFolderPaths m_result_folder_paths;
		// ----------------
	};

} // namespace ProjectName

#endif // RESOURCE_PROVIDER_H
