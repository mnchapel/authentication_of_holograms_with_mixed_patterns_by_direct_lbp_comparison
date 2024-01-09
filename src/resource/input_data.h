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

#ifndef INPUT_DATA_H
#define INPUT_DATA_H

// This
#include "resource/image_labels.h"
#include "utils/types/structs.h"
#include "utils/types/types_cpp.h"

// OpenCV
#include <opencv2/core.hpp>

namespace ProjectName {
	class ImageLabels;

	class InputData final
	{
	public:
		InputData() = delete;
		explicit InputData(const std::string& ovd_metadata_path);
		InputData(const InputData&) = delete;
		InputData(InputData&&) noexcept = delete;
		virtual ~InputData() = default;

		InputData& operator=(const InputData&) = delete;
		InputData& operator=(InputData&&) noexcept = delete;

		// -------
		// GETTERS
		inline size_t nbPatterns() const noexcept { return m_nb_patterns;}
		inline const std::string areaNum() const noexcept { return m_area_num; }
		inline const std::string& countryCode() const noexcept { return m_country_code; }
		inline const VecMat& cropMasks() const noexcept { return m_crop_masks; }
		inline const VecRect& cropRects() const noexcept { return m_crop_rects; }
		inline const std::string& docId() const noexcept { return m_doc_id; }
		inline const std::string& docLabel() const noexcept { return m_doc_label; }
		inline const std::string& docType() const noexcept { return m_doc_type; }
		inline const std::string& labelsFile() const noexcept { return m_labels_file; }
		inline const std::string& imageFolder() const noexcept { return m_img_folder; }
		inline const std::string& maskFolder() const noexcept { return m_mask_folder; }
		inline const std::string& metadataFile() const noexcept { return m_metadata_file; }
		inline const std::string& smartphone() const noexcept { return m_smartphone; }
		inline const std::string& videoName() const noexcept { return m_video_name; }
		inline const VecString& imageFilenameList() const noexcept { return m_filename_list; }
		inline const ImageLabels& imageLabels() const noexcept { return *(m_image_labels.get()); }

		inline int imageLabelAt(size_t img_num) const noexcept { return m_image_labels->labelAt(img_num); }
		inline std::string labelsTestFilePath(size_t label) const noexcept { return m_data_folder + m_labels_test_file_path[label]; }

		inline VecInt labelsTestAt(size_t label) const noexcept { return m_image_labels->labelsTest(label); }
		// -------

	private:
		void readCropFile(); // throw runtime_error
		void readCropMasksFile(); // throw runtime_error
		void readFilenamesFile(); // throw runtime_error
		void readHologramInfo(); // throw runtime_error
		void readInputFile(); // throw runtime_error
		void readLabelsFile(); // throw exception
		void readVideoMetadataFile(); // throw runtime_error
		void readOVDMetadataFile(const std::string& file_path); // throw runtime_error

		// ----------------
		// MEMBER VARIABLES
		size_t m_nb_patterns;
		std::string m_area_num;
		std::string m_country_code;
		std::string m_crop_masks_file;
		std::string m_data_folder;
		std::string m_doc_id;
		std::string m_doc_label;
		std::string m_doc_type;
		std::string m_img_folder;
		std::string m_filenames_file;
		std::string m_labels_file;
		std::string m_mask_folder;
		std::string m_metadata_file;
		std::string m_smartphone;
		std::string m_video_name;
		VecMat m_crop_masks;
		VecRect m_crop_rects;
		VecString m_filename_list;
		VecString m_labels_test_file_path;
		std::unique_ptr<ImageLabels> m_image_labels;
		// ----------------
	};

} // namespace ProjectName

#endif // INPUT_DATA_H
