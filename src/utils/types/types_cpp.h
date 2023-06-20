#ifndef TYPES_CPP_H
#define TYPES_CPP_H

// C++
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

namespace ProjectName {

	using USetSizeT = std::unordered_set<size_t>;

	using VecInt            = std::vector<int>;
	using VecFloat          = std::vector<float>;
	using VecDouble         = std::vector<double>;
	using VecString         = std::vector<std::string>;
	using VecVecInt         = std::vector<std::vector<int>>;
	using VecVecFloat       = std::vector<std::vector<float>>;
	using VecPairFloat      = std::vector<std::pair<float, float>>;
	using VecPairDouble     = std::vector<std::pair<double, double>>;
	using VecPairDoubleInt  = std::vector<std::pair<double, int>>;
	using VecTupleInt2SizeT = std::vector<std::tuple<int, size_t, size_t>>;

	using PairFloat     = std::pair<float, float>;
	using PairDouble    = std::pair<double, double>;
	using PairDoubleInt = std::pair<double, int>;

	using Tuple3Int        = std::tuple<int, int, int>;
	using Tuple2DoubleInt  = std::tuple<double, double, int>;
	using TupleInt2SizeT   = std::tuple<int, size_t, size_t>;
	using TupleFloatIntInt = std::tuple<float, int, int>;

}

#endif // TYPES_CPP_H
