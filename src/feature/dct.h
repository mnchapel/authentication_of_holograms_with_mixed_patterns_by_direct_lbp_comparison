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

#ifndef DCT_H
#define DCT_H

// This
#include "utils/types/types_cpp.h"

namespace cv {
	class Mat;
}

namespace ProjectName {

	class DCT final
	{
	public:
		explicit DCT() = default;
		DCT(const DCT&) = default;
		DCT(DCT&&) noexcept = default;
		virtual ~DCT() = default;

		DCT& operator=(const DCT&) = default;
		DCT& operator=(DCT&&) noexcept = default;

		VecDouble coarseCoefficients(const cv::Mat& img, int nb_rows, int nb_cols) noexcept;
		cv::Mat computeDCTCoeff(const cv::Mat& img) noexcept;
		cv::Mat computeIDCTCoeff(const cv::Mat& coeffs) noexcept;
		cv::Mat selectNZigZagCoefficients(const cv::Mat& coeffs, int first_coeff, int nb_coeff) noexcept;
	};

} // namespace ProjectName

#endif // DCT_H
