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

#include "printer/console_printer.h"

// C++
#include <iostream>
#include <stdlib.h>

namespace ProjectName {

	// ----------------------------------------------------------------------------
	ConsolePrinter& ConsolePrinter::instance() noexcept
	{
		static ConsolePrinter m_instance;
		return m_instance;
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updateMessage(const std::string& message) noexcept
	{
		std::cout << message << "\r" << std::flush;
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updatePatternName(const std::string& pattern_name) noexcept
	{
		m_pattern_name = pattern_name;
		updateConsoleVideoNames();
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updateProgressBar(const std::string& message, int value, int max_value) noexcept
	{
		int percent = value * 100 / max_value;
		std::cout << message << ": " << percent << "%\r" << std::flush;
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updateRefVideoName(const std::string& ref_video_name) noexcept
	{
		m_ref_video_name = ref_video_name;
		m_train_video_name = "";
		m_test_video_name = "";
		updateConsoleVideoNames();
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updateTestDocLabel(const std::string& doc_label) noexcept
	{
		m_test_doc_label = doc_label;
		updateConsoleVideoNames();
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updateTestVideoName(const std::string& test_video_name) noexcept
	{
		m_test_video_name = test_video_name;
		updateConsoleVideoNames();
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updateTrainVideoName(const std::string& train_video_name) noexcept
	{
		m_train_video_name = train_video_name;
		updateConsoleVideoNames();
	}

	// ----------------------------------------------------------------------------
	void ConsolePrinter::updateConsoleVideoNames() noexcept
	{
		system("cls"); // windows
		// system("clear"); // linux

		std::cout << "Ref " << m_ref_video_name;

		if (!m_train_video_name.empty())
			std::cout << " | Train " << m_train_video_name;

		if (!m_test_video_name.empty())
			std::cout << " | Test " << m_test_video_name << " " << m_test_doc_label;

		if (!m_pattern_name.empty())
			std::cout << " | Pattern " << m_pattern_name;

		std::cout << std::flush << std::endl;
	}

} // namespace ProjectName
