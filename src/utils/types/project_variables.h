#ifndef PROJECT_VARIABLES_H
#define PROJECT_VARIABLES_H

// C++
#include <string>

namespace ProjectName {

	struct ProjectFolder
	{
		inline static const std::string CONFIG = "../config/";
		inline static const std::string RESULTS = "../results/";
	};

	struct PlotStyle
	{
		inline static const std::string CLASSIFICATION_RESULT = "../config/sciplot_plot_styles/classification_result.yml";
		inline static const std::string LABELS = "../config/sciplot_plot_styles/labels.yml";
	};
	
}

#endif // PROJECT_VARIABLES_H
