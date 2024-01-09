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

#ifndef LP_FEATURE_H
#define LP_FEATURE_H

// This
#include "utils/types/types_cpp.h"
#include "utils/types/types_opencv.h"

namespace ProjectName {

	class LPFeature final
	{
	public:
		explicit LPFeature() = default;
		explicit LPFeature(const cv::Mat& dct_feature) noexcept;
		LPFeature(const LPFeature&) = default;
		LPFeature(LPFeature&&) noexcept = default;
		virtual ~LPFeature() = default;

		LPFeature& operator=(const LPFeature&) = delete;
		LPFeature& operator=(LPFeature&&) noexcept = delete;
		const std::vector<LP>& operator[](std::size_t index) const { return m_lp_model[index]; }

		// -------
		// GETTERS
		inline size_t nbRows() const noexcept { return m_lp_model.size(); }
		inline size_t nbCols() const noexcept { return m_lp_model.front().size(); }
		// -------

		cv::Mat createLPModelImage() const noexcept;
		int distance(const LPFeature& lp_model_test) const noexcept;
		PairFloat minDistanceLPModel(const LPFeature& lp_feature_test) const noexcept;

	private:
		uchar binaryValue(uchar intensity_1, uchar intensity_2) const noexcept;
		void createLPModel(const cv::Mat& img, const cv::Mat& mask = cv::Mat()) noexcept;
		PairMatInt createNonUniformLBPMat() const noexcept;
		int distanceLP(const LP& lp_1, const LP& lp_2) const noexcept;
		bool isUniformLP(const LP& lp) const noexcept;
		TupleFloatIntInt minDistanceLPNeighbors(const LP& lp_test, const cv::Point pixel_to_compare) const noexcept;
		uchar ternaryValue(uchar intensity_1, uchar intensity_2) const noexcept;

		// ----------------
		// MEMBER VARIABLES
		const VecPoint m_k_8_neighbor_points = {
			cv::Point{-1, -1},
			cv::Point{0, -1},
			cv::Point{1, -1},
			cv::Point{-1, 0},
			cv::Point{1, 0},
			cv::Point{-1, 1},
			cv::Point{0, 1},
			cv::Point{1, 1}};

		const VecPoint m_k_9_neighbor_points = {
			cv::Point{-1, -1},
			cv::Point{0, -1},
			cv::Point{1, -1},
			cv::Point{-1, 0},
			cv::Point{0, 0},
			cv::Point{1, 0},
			cv::Point{-1, 1},
			cv::Point{0, 1},
			cv::Point{1, 1}};

		LPModel m_lp_model;
		// ----------------
	};

} // namespace ProjectName

#endif // LP_FEATURE_H
