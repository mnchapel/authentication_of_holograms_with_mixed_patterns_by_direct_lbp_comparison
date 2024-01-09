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

#ifndef VIDEO_STATISTICS_H
#define VIDEO_STATISTICS_H

// This
#include "utils/types/types_cpp.h"

// C++
#include <filesystem>
#include <string>

namespace ProjectName {
	struct Statistics;

	class VideoStatistics final
	{

	public:
		explicit VideoStatistics() = default;
		VideoStatistics(const VideoStatistics&) = default;
		VideoStatistics(VideoStatistics&&) noexcept = default;
		virtual ~VideoStatistics() = default;

		VideoStatistics& operator=(const VideoStatistics&) = default;
		VideoStatistics& operator=(VideoStatistics&&) noexcept = default;

	private:
	};

} // namespace ProjectName

#endif // VIDEO_STATISTICS_H
