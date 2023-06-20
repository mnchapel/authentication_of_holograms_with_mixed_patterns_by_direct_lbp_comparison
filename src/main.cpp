// This
#include "authentication/application.h"

// C++
#include <iostream>

// ----------------------------------------------------------------------------
int main()
{
	try
	{
		ProjectName::Application application;
		application.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
