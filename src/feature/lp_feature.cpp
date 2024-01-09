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

#include "feature/lp_feature.h"

// This
#include "resource/resource_provider.h"
#include "utils/algorithm.h"

namespace ProjectName {

	// ----------------------------------------------------------------------------
	LPFeature::LPFeature(const cv::Mat& dct_feature) noexcept
	{
		createLPModel(dct_feature);
	}

	// ----------------------------------------------------------------------------
	uchar LPFeature::binaryValue(uchar intensity_1, uchar intensity_2) const noexcept
	{
		uchar binary_value = 1;
		if (intensity_1 > intensity_2)
			binary_value = 0;

		return binary_value;
	}

	// ----------------------------------------------------------------------------
	void LPFeature::createLPModel(const cv::Mat& img, const cv::Mat& mask) noexcept
	{
		LP neighbors(m_k_8_neighbor_points.size(), 1);
		VecVecUchar lp_model_cols(static_cast<size_t>(img.cols), neighbors);
		m_lp_model = LPModel(static_cast<size_t>(img.rows), lp_model_cols);

		size_t nb_rows = static_cast<size_t>(img.rows);
		size_t nb_cols = static_cast<size_t>(img.cols);
		for (size_t r = 0; r < nb_rows; r++)
			for (size_t c = 0; c < nb_cols; c++)
			{
				cv::Point p{static_cast<int>(c), static_cast<int>(r)};
				if (!mask.empty() && mask.at<uchar>(p) == 0)
					continue;

				uchar intensity = img.at<uchar>(p);

				for (size_t neighbor_id = 0; neighbor_id < m_k_8_neighbor_points.size(); neighbor_id++)
				{
					int r_n = static_cast<int>(r) + m_k_8_neighbor_points[neighbor_id].y;
					int c_n = static_cast<int>(c) + m_k_8_neighbor_points[neighbor_id].x;

					if (isPointOutOfImageBounds(img, r_n, c_n))
						continue;

					uchar intensity_n = img.at<uchar>(r_n, c_n);

					cv::Point p_n{static_cast<int>(c_n), static_cast<int>(r_n)};
					if (!mask.empty() && mask.at<uchar>(p_n) == 0)
						continue;

					m_lp_model[r][c][neighbor_id] = binaryValue(intensity, intensity_n);
				}
			}
	}

	// ----------------------------------------------------------------------------
	cv::Mat LPFeature::createLPModelImage() const noexcept
	{
		size_t nb_rows = nbRows();
		size_t nb_cols = nbCols();

		VecUchar colors = {170, 85, 0};

		int size_neighbor_square = static_cast<int>(sqrt(static_cast<int>(m_k_9_neighbor_points.size()) + 1));

		cv::Size lp_model_img_size{static_cast<int>(nb_cols) * size_neighbor_square, static_cast<int>(nb_rows) * size_neighbor_square};
		cv::Mat lp_model_img(lp_model_img_size, CV_8UC1, cv::Scalar{85});

		for (size_t r = 0; r < nb_rows; r++)
			for (size_t c = 0; c < nb_cols; c++)
				for (size_t n = 0; n < m_k_8_neighbor_points.size(); n++)
				{
					int r_sq = (static_cast<int>(r) * size_neighbor_square + size_neighbor_square / 2) + m_k_8_neighbor_points[n].y;
					int c_sq = (static_cast<int>(c) * size_neighbor_square + size_neighbor_square / 2) + m_k_8_neighbor_points[n].x;

					size_t color_id = static_cast<size_t>(m_lp_model[r][c][n]);
					lp_model_img.at<uchar>(r_sq, c_sq) = colors[color_id];
				}

		return lp_model_img;
	}

	// ----------------------------------------------------------------------------
	// Non Uniform, not as in paper (for corners, edges) but uniform like with the same values
	PairMatInt LPFeature::createNonUniformLBPMat() const noexcept
	{
		size_t nb_rows = nbRows();
		size_t nb_cols = nbCols();
		cv::Size mat_size{static_cast<int>(nb_cols), static_cast<int>(nb_rows)};
		cv::Mat non_uniform(mat_size, CV_8UC1, cv::Scalar{0});
		int count_nb_non_uniform = 0;

		for (size_t r = 0; r < nb_rows; r++)
			for (size_t c = 0; c < nb_cols; c++)
			{
				const LP& lp = m_lp_model[r][c];

				if (!isUniformLP(lp))
				{
					cv::Point p{static_cast<int>(c), static_cast<int>(r)};
					non_uniform.at<uchar>(p) = 1;
					count_nb_non_uniform++;
				}
			}

		return std::make_pair(non_uniform, count_nb_non_uniform);
	}

	// ----------------------------------------------------------------------------
	int LPFeature::distance(const LPFeature& lp_model_test) const noexcept
	{
		assert(nbRows() == lp_model_test.nbRows());
		assert(nbCols() == lp_model_test.nbCols());

		int nb_rows = static_cast<int>(nbRows());
		int nb_cols = static_cast<int>(nbCols());

		int dist_lp_model = 0;
		for (int r = 0; r < nb_rows; r++)
			for (int c = 0; c < nb_cols; c++)
			{
				int dist_min_neighbors = INT_MAX;
				for (size_t n = 0; n < m_k_9_neighbor_points.size(); n++)
				{
					size_t r_n = static_cast<size_t>(r + m_k_9_neighbor_points[n].y);
					size_t c_n = static_cast<size_t>(c + m_k_9_neighbor_points[n].x);

					if (isPointOutOfModel<LPModel>(m_lp_model, r_n, c_n))
						continue;

					int dist_lp = distanceLP(m_lp_model[r_n][c_n], lp_model_test[r_n][c_n]);
					dist_min_neighbors = std::min(dist_min_neighbors, dist_lp);
				}

				dist_lp_model += dist_min_neighbors;
			}

		return dist_lp_model;
	}

	// ----------------------------------------------------------------------------
	int LPFeature::distanceLP(const LP& lp_1, const LP& lp_2) const noexcept
	{
		assert(lp_1.size() == lp_2.size());

		size_t nb_elems = lp_1.size();

		int diff = 0;
		for (size_t i = 0; i < nb_elems; i++)
			diff += abs(lp_1[i] - lp_2[i]);

		return diff;
	}

	// ----------------------------------------------------------------------------
	bool LPFeature::isUniformLP(const LP& lp) const noexcept
	{
		bool is_uniform = true;
		int first_val = lp.front();

		size_t nb_elems = lp.size();
		for (size_t lp_id = 1; lp_id < nb_elems; lp_id++)
			if (first_val != lp[lp_id])
				is_uniform = false;

		return is_uniform;
	}

	// ----------------------------------------------------------------------------
	PairFloat LPFeature::minDistanceLPModel(const LPFeature& lp_feature_test) const noexcept
	{
		float sum_img_dist = 0.f;
		int count_model = 0;
		cv::Mat non_uniform_model;
		int count_non_uniform = 0;

		std::tie(non_uniform_model, count_non_uniform) = createNonUniformLBPMat();

		size_t nb_rows = nbRows();
		size_t nb_cols = nbCols();

		for (size_t r = 0; r < nb_rows; r++)
			for (size_t c = 0; c < nb_cols; c++)
			{
				const LP& lp_test = lp_feature_test[r][c];

				int best_r_n = -1;
				int best_c_n = -1;
				float min_dist_neigh = FLT_MAX;
				cv::Point p{static_cast<int>(c), static_cast<int>(r)};
				std::tie(min_dist_neigh, best_r_n, best_c_n) = minDistanceLPNeighbors(lp_test, p);

				if (non_uniform_model.at<uchar>(best_r_n, best_c_n) == 1)
				{
					count_model++;
					non_uniform_model.at<uchar>(best_r_n, best_c_n) = 0;
				}

				sum_img_dist += min_dist_neigh;
			}

		return std::make_pair(sum_img_dist, (static_cast<float>(count_model) / static_cast<float>(count_non_uniform)) * 100.f);
	}

	// ----------------------------------------------------------------------------
	TupleFloatIntInt LPFeature::minDistanceLPNeighbors(const LP& lp_test, const cv::Point pixel_to_compare) const noexcept
	{
		int best_r_n = -1;
		int best_c_n = -1;

		int min_dist_neigh = INT_MAX;
		for (size_t n = 0; n < m_k_9_neighbor_points.size(); n++)
		{
			size_t r_n = static_cast<size_t>(pixel_to_compare.y + m_k_9_neighbor_points[n].y);
			size_t c_n = static_cast<size_t>(pixel_to_compare.x + m_k_9_neighbor_points[n].x);

			if (isPointOutOfModel<LPModel>(m_lp_model, r_n, c_n))
				continue;

			const LP& lp_model = m_lp_model[r_n][c_n];
			int dist = distanceLP(lp_model, lp_test);

			if (min_dist_neigh > dist)
			{
				best_r_n = static_cast<int>(r_n);
				best_c_n = static_cast<int>(c_n);
				min_dist_neigh = dist;
			}
		}

		return std::make_tuple(static_cast<float>(min_dist_neigh), best_r_n, best_c_n);
	}

	// ----------------------------------------------------------------------------
	uchar LPFeature::ternaryValue(uchar intensity_1, uchar intensity_2) const noexcept
	{
		uchar ternary_value = 1;
		if (abs(intensity_1 - intensity_2) < 30)
			ternary_value = 1;
		else if (intensity_1 > intensity_2)
			ternary_value = 2;
		else if (intensity_1 < intensity_2)
			ternary_value = 0;

		return ternary_value;
	}

} // namespace ProjectName