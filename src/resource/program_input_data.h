#ifndef PROGRAM_INPUT_DATA_H
#define PROGRAM_INPUT_DATA_H

// This
#include "utils/types/types_cpp.h"

namespace ProjectName {
	class ProgramInputData final
	{
	public:
		ProgramInputData();
		ProgramInputData(const ProgramInputData&) = default;
		ProgramInputData(ProgramInputData&&) noexcept = default;
		virtual ~ProgramInputData() = default;

		ProgramInputData& operator=(const ProgramInputData&) = default;
		ProgramInputData& operator=(ProgramInputData&&) noexcept = default;

		// -------
		// GETTERS
		inline std::string refOVD() const noexcept { return m_ref_ovd; }
		inline VecString testOVD() const noexcept { return m_test_ovd; }
		inline VecString trainOVD() const noexcept { return m_train_ovd; }
		// -------

	private:
		// ----------------
		// MEMBER VARIABLES
		std::string m_ref_ovd;
		VecString m_train_ovd;
		VecString m_test_ovd;
		// ----------------
	};

} // namespace ProjectName

#endif // PROGRAM_INPUT_DATA_H
