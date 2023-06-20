#include "authentication/application.h"

// This
#include "authentication/lbp_authentication.h"
#include "printer/console_printer.h"
#include "printer/global_statistics_report.h"
#include "resource/input_data.h"
#include "resource/program_input_data.h"
#include "resource/resource_provider.h"
#include "statistics/global_statistics.h"
#include "utils/algorithm.h"

// C++
#include <chrono>

namespace ProjectName {

	// ----------------------------------------------------------------------------
	VecString Application::getFilenames(const std::string& img_test_folder)
	{
		std::string filename = img_test_folder + "../filenames.yml";
		cv::FileStorage fs{filename, cv::FileStorage::READ};

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + filename + " cannot be opened."};

		VecString img_test_filenames;
		cv::read(fs["filenames"], img_test_filenames);

		fs.release();

		return img_test_filenames;
	}

	// ----------------------------------------------------------------------------
	void Application::printStatistics(ResourceProvider& resource_provider)
	{
		try
		{
			std::string output_folder = resource_provider.requestResultFolderRef();
			fs::path output_folder_path{output_folder};
			output_folder = output_folder_path.parent_path().parent_path().string();

			GlobalStatistics global_statistics{output_folder};

			GlobalStatisticsReport global_stat_report;
			global_stat_report.write(output_folder, global_statistics);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void Application::run()
	{
		try
		{
			setCursorVisibility(false);

			ResourceProvider& resource_provider = ResourceProvider::instance();
			const ProgramInputData& program_input_data = resource_provider.requestProgramInputData();
			
			LBPAuthentication authentication;
			runReference(authentication, resource_provider);

			for (const std::string& train_ovd_file_path : program_input_data.trainOVD())
			{
			  	std::vector<TrainResult> train_results = runTrain(authentication, resource_provider, train_ovd_file_path);

			 	for (const std::string& test_ovd_file_path : program_input_data.testOVD())
			 		runTest(authentication, resource_provider, test_ovd_file_path, train_results);
			}

			printStatistics(resource_provider);

			setCursorVisibility(true);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void Application::runReference(LBPAuthentication& authentication, ResourceProvider& resource_provider) noexcept
	{
		try
		{
			const ProgramInputData& program_input_data = resource_provider.requestProgramInputData();
			const InputData& input_data = resource_provider.requestRefOVD(program_input_data.refOVD());

			ConsolePrinter& console = ConsolePrinter::instance();
			console.updateRefVideoName(input_data.videoName());

			const std::string& output_folder = resource_provider.requestResultFolderRef();
			authentication.initMethodParameters(input_data);
			authentication.computeRef(input_data, output_folder);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	void Application::runTest(LBPAuthentication& authentication, ResourceProvider& resource_provider, const std::string& test_ovd_file_path, const std::vector<TrainResult>& train_results)
	{
		try
		{
			const InputData& input_data = resource_provider.requestTestOVD(test_ovd_file_path);

			ConsolePrinter& console = ConsolePrinter::instance();
			console.updateTestVideoName(input_data.videoName());
			console.updateTestDocLabel(input_data.docLabel());

			const std::string& output_folder = resource_provider.requestResultFolderTest();
			authentication.computeTest(input_data, output_folder, train_results);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}

	// ----------------------------------------------------------------------------
	std::vector<TrainResult> Application::runTrain(LBPAuthentication& authentication, ResourceProvider& resource_provider, const std::string& train_ovd_file_path) noexcept
	{
		std::vector<TrainResult> train_results;

		const InputData& train_ovd = resource_provider.requestTrainOVD(train_ovd_file_path);

		ConsolePrinter& console = ConsolePrinter::instance();
		console.updateTrainVideoName(train_ovd.videoName());

		const std::string& output_folder = resource_provider.requestResultFolderTrain();
		train_results = authentication.computeTrain(train_ovd, output_folder);

		return train_results;
	}

} // namespace ProjectName
