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

#ifndef CONSOLE_PRINTER_H
#define CONSOLE_PRINTER_H

// C++
#include <string>

namespace ProjectName {

	class ConsolePrinter final
	{
	public:
		static ConsolePrinter& instance() noexcept;

		void updateMessage(const std::string& message) noexcept;
		void updatePatternName(const std::string& pattern_name) noexcept;
		void updateProgressBar(const std::string& message, int value, int max_value) noexcept;
		void updateRefVideoName(const std::string& ref_video_name) noexcept;
		void updateTestDocLabel(const std::string& doc_label) noexcept;
		void updateTestVideoName(const std::string& test_video_name) noexcept;
		void updateTrainVideoName(const std::string& train_video_name) noexcept;

	private:
		explicit ConsolePrinter() = default;
		ConsolePrinter(const ConsolePrinter&) = delete;
		ConsolePrinter(ConsolePrinter&&) noexcept = delete;
		virtual ~ConsolePrinter() = default;

		ConsolePrinter& operator=(const ConsolePrinter&) = delete;
		ConsolePrinter& operator=(ConsolePrinter&&) noexcept = delete;

		void updateConsoleVideoNames() noexcept;

		std::string m_pattern_name;
		std::string m_ref_video_name;
		std::string m_train_video_name;
		std::string m_test_doc_label;
		std::string m_test_video_name;
	};

} // namespace ProjectName

#endif // CONSOLE_PRINTER_H
