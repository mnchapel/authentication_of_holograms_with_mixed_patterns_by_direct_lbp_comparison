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
