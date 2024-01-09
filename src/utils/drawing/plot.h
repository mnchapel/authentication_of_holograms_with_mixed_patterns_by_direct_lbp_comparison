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

#ifndef PLOT_H
#define PLOT_H

// C++
#include "utils/types/types_sciplot.h"
#include "utils/types/structs.h"

namespace sciplot {
	class Plot;
}

namespace ProjectName {

	class Plot final
	{
	public:
		explicit Plot() = default;
		Plot(const Plot&) = default;
		Plot(Plot&&) noexcept = default;
		virtual ~Plot() = default;

		Plot& operator=(const Plot&) = default;
		Plot& operator=(Plot&&) noexcept = default;

		void scatterAndSave(const std::vector<LPModelMetric>& metrics, const VecInt& labels, const std::string& output_filename) noexcept;
		void scatterAndSave(const cv::Mat& metrics, const VecInt& labels, const std::string& output_filename) noexcept;
		void scatterAndShow(const std::vector<LPModelMetric>& metrics, const VecInt& labels) noexcept;
		void setPlotStyle(const std::string& style); // throw runtime_error

	private:
		void adjustAxis(sciplot::Plot& plot, const std::vector<std::vector<LPModelMetric>>& separated_metrics) noexcept;
		void configurePlot(sciplot::Plot& plot) noexcept;
		PairSciplotVec convertType(const std::vector<LPModelMetric>& metrics) noexcept;
		std::vector<std::vector<LPModelMetric>> prepareDataForScatter(const std::vector<LPModelMetric>& metrics, const VecInt& labels) noexcept;
		std::vector<std::vector<LPModelMetric>> prepareDataForScatter(const cv::Mat& metrics, const VecInt& labels) noexcept;
		sciplot::Plot scatter(const std::vector<std::vector<LPModelMetric>>& separated_metrics) noexcept;

		// ----------------
		// MEMBER VARIABLES
		// ----------------
		VecString m_label_colors;
		VecString m_label_labels;
	};
}

#endif // PLOT_H