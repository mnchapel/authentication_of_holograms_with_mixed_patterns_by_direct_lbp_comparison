#ifndef OPERATOR_H
#define OPERATOR_H

// This
#include "utils/types/types_cpp.h"

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/core/persistence.hpp>

namespace ProjectName {

	// ----------------------------------------------------------------------------
	inline void operator>>(const cv::FileNode& node, size_t& values)
	{
		int val = node;
		values = static_cast<size_t>(val);
	}

	// ----------------------------------------------------------------------------
	inline void operator>>(const cv::FileNode& node, PairFloat& values)
	{
		values.first = node[0];
		values.second = node[1];
	}

	// ----------------------------------------------------------------------------
	inline void operator>>(const cv::FileNode& node, VecTupleInt2SizeT& values)
	{
		cv::FileNodeIterator it = node.begin();
		cv::FileNodeIterator it_end = node.end();

		for (; it != it_end; ++it)
		{
			cv::FileNode tuple_node = *it;

			Tuple3Int tuple;

			std::get<0>(tuple) = tuple_node[0];
			std::get<1>(tuple) = tuple_node[1];
			std::get<2>(tuple) = tuple_node[2];

			values.push_back(tuple);
		}
	}

	// ----------------------------------------------------------------------------
	inline void operator>>(const cv::FileNode& node, cv::Rect& rect)
	{
		rect.x = node[0];
		rect.y = node[1];
		rect.width = node[2];
		rect.height = node[3];
	}

	// ----------------------------------------------------------------------------
	inline cv::FileStorage& operator<<(cv::FileStorage& fs, const PairFloat& values)
	{
		fs << "[:" << values.first << values.second << "]";

		return fs;
	}

	// ----------------------------------------------------------------------------
	inline cv::FileStorage& operator<<(cv::FileStorage& fs, const VecTupleInt2SizeT& values)
	{
		fs << "[:";
		size_t nb_tuples = values.size();
		for (size_t i = 0; i < nb_tuples; i++)
		{
			const Tuple3Int& tuple = values[i];
			fs << "[:" << std::get<0>(tuple) << std::get<1>(tuple) << std::get<2>(tuple) << "]";
		}
		fs << "]";

		return fs;
	}

	// ----------------------------------------------------------------------------
	inline cv::FileStorage& operator<<(cv::FileStorage& fs, const cv::Rect& rect)
	{
		fs << "[:" << rect.x << rect.y << rect.width << rect.height << "]";

		return fs;
	}

} // namespace ProjectName

#endif // OPERATOR_H
