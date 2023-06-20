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
