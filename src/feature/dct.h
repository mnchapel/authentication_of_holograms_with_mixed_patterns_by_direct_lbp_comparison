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
