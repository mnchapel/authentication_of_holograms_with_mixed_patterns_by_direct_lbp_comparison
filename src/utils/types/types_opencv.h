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
