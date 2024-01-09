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

#ifndef APPLICATION_H
#define APPLICATION_H

// This
#include "resource/image_labels.h"
#include "resource/input_data.h"

// C++
#include <string>

namespace ProjectName {
	
	struct TrainResult;
	class Authentication;
	class LBPAuthentication;
	class ResourceProvider;

	class Application final
	{
	public:
		explicit Application() noexcept = default;
		Application(const Application&) = default;
		Application(Application&&) noexcept = default;
		virtual ~Application() = default;

		Application& operator=(const Application&) = default;
		Application& operator=(Application&&) noexcept = default;

		void run(); // throw runtime_error

	private:
		VecString getFilenames(const std::string& img_test_folder); // throw runtime_error
		void printStatistics(ResourceProvider& resource_provider); // throw std::exception
		void runReference(LBPAuthentication& authentication, ResourceProvider& resource_provider) noexcept;
		void runTest(LBPAuthentication& authentication, ResourceProvider& resource_provider, const std::string& test_ovd_file_path, const std::vector<TrainResult>& train_results);
		std::vector<TrainResult> runTrain(LBPAuthentication& authentication, ResourceProvider& resource_provider, const std::string& train_ovd_file_path) noexcept;
	};

} // namespace ProjectName

#endif // APPLICATION_H
