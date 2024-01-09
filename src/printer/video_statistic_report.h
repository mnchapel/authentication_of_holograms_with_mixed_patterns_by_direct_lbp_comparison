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

#ifndef VIDEO_STATISTIC_REPORT_H
#define VIDEO_STATISTIC_REPORT_H

// This
#include "utils/types/types_cpp.h"

// C++
#include <string>

namespace ProjectName {
	struct AuthResults;
	class InputData;

	class VideoStatisticReport final
	{
	public:
		explicit VideoStatisticReport() = default;
		VideoStatisticReport(const VideoStatisticReport&) = default;
		VideoStatisticReport(VideoStatisticReport&&) noexcept = default;
		virtual ~VideoStatisticReport() = default;

		VideoStatisticReport& operator=(const VideoStatisticReport&) = default;
		VideoStatisticReport& operator=(VideoStatisticReport&&) noexcept = default;

		void write(const std::string& output_folder, const InputData& input_data, const AuthResults& auth_results, const int label_ref);

	private:
		struct PredictiveValue
		{
			int m_nb_tp;
			int m_nb_tn;
			int m_nb_fp;
			int m_nb_fn;

			PredictiveValue()
				: m_nb_tp{0}
				, m_nb_tn{0}
				, m_nb_fp{0}
				, m_nb_fn{0}
			{
			}
		};

		struct ResultStatistics
		{
			float m_precision;
			float m_recall;

			ResultStatistics()
				: m_precision{0.f}
				, m_recall{0.f}
			{
			}
		};

		PredictiveValue computePredictiveValue(const VecInt& labels, const VecInt& predicted_labels, const int label_ref) noexcept;
		ResultStatistics computeResultStatistics(const PredictiveValue& pv) noexcept;

		// ----------------
		// MEMBER VARIABLES
		VecInt m_labels;
		VecInt m_predicted_labels;
		// ----------------
	};

} // namespace ProjectName

#endif // STATISTIC_REPORT_H
