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
