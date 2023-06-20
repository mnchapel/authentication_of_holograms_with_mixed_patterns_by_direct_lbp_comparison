#include "feature/img_feature.h"

namespace ProjectName {
	// ----------------------------------------------------------------------------
	ImgFeature::ImgFeature(const cv::Mat& dct_feature) noexcept
		: m_dct_feature{dct_feature}
		, m_lp_feature{LPFeature{dct_feature}}
	{
	}
	
	// ----------------------------------------------------------------------------
	cv::Mat ImgFeature::lpImage() const noexcept
	{
		return m_lp_feature.createLPModelImage();
	}

} // namespace ProjectName