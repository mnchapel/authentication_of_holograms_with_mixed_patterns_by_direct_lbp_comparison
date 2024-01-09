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