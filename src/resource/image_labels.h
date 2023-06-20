#ifndef IMAGE_LABELS_H
#define IMAGE_LABELS_H

// This
#include "utils/types/types_cpp.h"

// OpenCV
#include <opencv2/core.hpp>

namespace ProjectName {
	class InputData;

	class ImageLabels final
	{
	public:
		ImageLabels() = delete;
		explicit ImageLabels(InputData& input_data);
		ImageLabels(const ImageLabels&) = default;
		ImageLabels(ImageLabels&&) noexcept = default;
		virtual ~ImageLabels() = default;

		ImageLabels& operator=(const ImageLabels&) = default;
		ImageLabels& operator=(ImageLabels&&) noexcept = default;

		// -------
		// GETTERS
		VecInt labels() const noexcept { return m_labels_ref; }
		int labelAt(size_t img_num) const noexcept { return m_labels_ref[img_num]; }
		VecInt labelsTest(size_t label) const noexcept { return m_labels_test[label - 1]; }
		int labelsTestAt(size_t label, size_t img_num) const noexcept { return m_labels_test[label - 1][img_num]; }
		// -------

	private:
		VecInt load(const std::string& path); // throw runtime_error

		// ----------------
		// MEMBER VARIABLES
		VecInt m_labels_ref;
		VecVecInt m_labels_test;
		// ----------------
	};

} // namespace ProjectName

#endif // IMAGE_LABELS_H
