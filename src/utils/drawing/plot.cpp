#include "utils/drawing/plot.h"

// Include sciplot before windows.h (in algorithm.h) -> it is a known issue
// sciplot
//#include <sciplot/sciplot.hpp>

// This
#include "resource/image_labels.h"
#include "utils/types/structs.h"

// sciplot
#include <sciplot/sciplot.hpp>

#include <iostream>

namespace ProjectName {

	// ----------------------------------------------------------------------------
	void Plot::adjustAxis(sciplot::Plot& plot, const std::vector<std::vector<LPModelMetric>>& separated_metrics) noexcept
	{

		double max_y_value = 0.;

		for(const std::vector<LPModelMetric>& vec_metrics : separated_metrics)
			for(const LPModelMetric& metric : vec_metrics)
			max_y_value = (metric.m_dist_to_multi_ref > max_y_value) ? metric.m_dist_to_multi_ref : max_y_value;

		double max_y_range = std::max(max_y_value, 1.);
		plot.xrange(0., 1.);
		plot.yrange(0., max_y_range);
	}

	// ----------------------------------------------------------------------------
	void Plot::configurePlot(sciplot::Plot& plot) noexcept
	{
		plot.xlabel("Distance Non Uniform");
		plot.ylabel("Distance to Multi LBP Model");
		plot.legend()
			.atOutsideRight()
			.displayVertical()
			.displaySpacing(2);

		plot.size(700, 500);
	}

	// ----------------------------------------------------------------------------
	PairSciplotVec Plot::convertType(const std::vector<LPModelMetric>& metrics) noexcept
	{
		sciplot::Vec x;
		sciplot::Vec y;

		size_t nb_elems = metrics.size();
		x.resize(nb_elems);
		y.resize(nb_elems);

		for (size_t i = 0; i < nb_elems; i++)
		{
			x[i] = static_cast<double>(metrics[i].m_percent_non_uniform);
			y[i] = static_cast<double>(metrics[i].m_dist_to_multi_ref);
		}

		return std::make_pair(x, y);
	}

	// ----------------------------------------------------------------------------
	void Plot::scatterAndSave(const std::vector<LPModelMetric>& metrics, const VecInt& labels, const std::string& output_filename) noexcept
	{
		std::vector<std::vector<LPModelMetric>> separated_metrics = prepareDataForScatter(metrics, labels);
		sciplot::Plot plot = scatter(separated_metrics);
		adjustAxis(plot, separated_metrics);
		plot.save(output_filename);
	}

	// ----------------------------------------------------------------------------
	void Plot::scatterAndSave(const cv::Mat& metrics, const VecInt& labels, const std::string& output_filename) noexcept
	{
		std::vector<std::vector<LPModelMetric>> separated_metrics = prepareDataForScatter(metrics, labels);
		sciplot::Plot plot = scatter(separated_metrics);
		adjustAxis(plot, separated_metrics);
		plot.save(output_filename);
	}

	// ----------------------------------------------------------------------------
	void Plot::scatterAndShow(const std::vector<LPModelMetric>& metrics, const VecInt& labels) noexcept
	{
		std::vector<std::vector<LPModelMetric>> separated_metrics = prepareDataForScatter(metrics, labels);
		sciplot::Plot plot = scatter(separated_metrics);
		adjustAxis(plot, separated_metrics);
		plot.show();
	}

	// ----------------------------------------------------------------------------
	std::vector<std::vector<LPModelMetric>> Plot::prepareDataForScatter(const std::vector<LPModelMetric>& metrics, const VecInt& labels) noexcept
	{
		size_t max_label = static_cast<size_t>(*std::max_element(labels.begin(), labels.end()));
		std::vector<std::vector<LPModelMetric>> separated_metrics(max_label + 1);

		const size_t nb_elems = metrics.size();
		for (size_t i = 0; i < nb_elems; i++)
		{
			size_t label = static_cast<size_t>(labels[i]);
			separated_metrics[label].push_back(metrics[i]);
		}

		return separated_metrics;
	}

	// ----------------------------------------------------------------------------
	std::vector<std::vector<LPModelMetric>> Plot::prepareDataForScatter(const cv::Mat& metrics, const VecInt& labels) noexcept
	{
		assert(static_cast<size_t>(metrics.rows) == labels.size());

		double max_label;
		cv::minMaxLoc(labels, static_cast<double*>(0), &max_label);
		std::vector<std::vector<LPModelMetric>> separated_metrics(static_cast<size_t>(max_label) + 1);

		const size_t nb_elems = static_cast<size_t>(metrics.rows);
		for (size_t i = 0; i < nb_elems; i++)
		{
			size_t label = static_cast<size_t>(labels[i]);
			cv::Point p1{0, static_cast<int>(i)};
			cv::Point p2{1, static_cast<int>(i)};
			separated_metrics[label].push_back(LPModelMetric{metrics.at<float>(p1), metrics.at<float>(p2)});
		}

		return separated_metrics;
	}

	// ----------------------------------------------------------------------------
	void Plot::setPlotStyle(const std::string& style_path)
	{
		cv::FileStorage fs(style_path, cv::FileStorage::READ);

		if (!fs.isOpened())
			throw std::runtime_error{"The file " + style_path + " cannot be opened."};

		cv::read(fs["label_colors"], m_label_colors);
		cv::read(fs["label_labels"], m_label_labels);

		fs.release();
	}

	// ----------------------------------------------------------------------------
	sciplot::Plot Plot::scatter(const std::vector<std::vector<LPModelMetric>>& separated_metrics) noexcept
	{
		sciplot::Plot plot;
		configurePlot(plot);

		size_t nb_elems = separated_metrics.size();
		for (size_t i = 0; i < nb_elems; i++)
		{
			const std::vector<LPModelMetric>& one_separated_metrics = separated_metrics[i];
			if (one_separated_metrics.size() == 0)
				continue;

			sciplot::Vec x, y;
			std::tie(x, y) = convertType(one_separated_metrics);

			plot.drawPoints(x, y)
				.pointType(1)
				.lineColor(m_label_colors[i])
				.pointSize(1)
				.label(m_label_labels[i]);
		}

		return plot;
	}

} // namespace ProjectName
