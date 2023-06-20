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
