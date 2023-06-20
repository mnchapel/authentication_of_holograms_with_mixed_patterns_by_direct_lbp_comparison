#ifndef RESULT_FOLDER_PATHS_H
#define RESULT_FOLDER_PATHS_H

// C++
#include <string>

namespace ProjectName {
	class InputData;

	class ResultFolderPaths final
	{
	public:
		explicit ResultFolderPaths() = default;
		ResultFolderPaths(const ResultFolderPaths&) = default;
		ResultFolderPaths(ResultFolderPaths&&) noexcept = default;
		virtual ~ResultFolderPaths() = default;

		ResultFolderPaths& operator=(const ResultFolderPaths&) = default;
		ResultFolderPaths& operator=(ResultFolderPaths&&) noexcept = default;

		// -------
		// GETTERS
		inline std::string resultFolderRefArea() const noexcept { return m_ref_area; }
		inline std::string resultFolderTestArea() const noexcept { return m_ref + m_train + m_test_area; }
		inline std::string resultFolderTrainArea() const noexcept { return m_ref + m_train_area; }
		// -------

		void updateRef(const InputData& input_data) noexcept;
		void updateTest(const InputData& input_data) noexcept;
		void updateTrain(const InputData& input_data) noexcept;

	private:
		void updatePatterns(const std::string& folder_path, const size_t nb_patterns) noexcept;

		// ----------------
		// MEMBER VARIABLES
		std::string m_ref;
		std::string m_ref_area;
		std::string m_test;
		std::string m_test_area;
		std::string m_train;
		std::string m_train_area;
		// ----------------
	};

} // namespace ProjectName

#endif // RESULT_FOLDER_PATHS_H
