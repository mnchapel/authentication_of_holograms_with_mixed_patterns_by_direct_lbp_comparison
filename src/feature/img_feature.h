#ifndef IMG_FEATURE_H
#define IMG_FEATURE_H

// This
#include "feature/lp_feature.h"
#include "utils/types/types_opencv.h"

namespace ProjectName {

	class ImgFeature final
	{
	public:
		explicit ImgFeature() = default;
		explicit ImgFeature(const cv::Mat& dct_feature) noexcept;
		ImgFeature(const ImgFeature&) = default;
		ImgFeature(ImgFeature&&) noexcept = default;
		virtual ~ImgFeature() = default;

		ImgFeature& operator=(const ImgFeature&) = delete;
		ImgFeature& operator=(ImgFeature&&) noexcept = delete;

		// -------
		// GETTERS
		inline const cv::Mat& dctFeature() const noexcept { return m_dct_feature; }
		inline const LPFeature& lpFeature() const noexcept { return m_lp_feature; }
		// -------
		
		cv::Mat lpImage() const noexcept;

	private:
		// ----------------
		// MEMBER VARIABLES
		cv::Mat m_dct_feature;
		LPFeature m_lp_feature;
		// ----------------
	};
	
	using PatternFeature = std::vector<ImgFeature>;
	using HoloFeature = std::vector<PatternFeature>;

} // namespace ProjectName

#endif // IMG_FEATURE_H
