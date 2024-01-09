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
