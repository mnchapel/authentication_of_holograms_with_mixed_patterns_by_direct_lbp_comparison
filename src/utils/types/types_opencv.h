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

#ifndef TYPES_OPENCV_H
#define TYPES_OPENCV_H

// C++
#include <utility>
#include <vector>

// OpenCV
#include <opencv2/core/core_c.h>

namespace ProjectName {

	using VecUchar    = std::vector<uchar>;
	using VecScalar   = std::vector<cv::Scalar>;
	using VecPoint    = std::vector<cv::Point>;
	using VecRect     = std::vector<cv::Rect>;
	using VecMat      = std::vector<cv::Mat>;
	using VecVecUchar = std::vector<std::vector<uchar>>;

	using PairMatInt = std::pair<cv::Mat, int>;

	using LP           = std::vector<uchar>;
	using LPModel      = std::vector<std::vector<LP>>;
	using MultiLPModel = std::vector<LPModel>;

}

#endif // TYPES_OPENCV_H
