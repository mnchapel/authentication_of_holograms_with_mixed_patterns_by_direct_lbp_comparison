#include "feature/dct.h"

// This
#include "utils/algorithm.h"

// C++
#include <iostream>

// OpenCV
#include <opencv2/imgproc.hpp>


namespace ProjectName
{
    // ----------------------------------------------------------------------------
    VecDouble DCT::coarseCoefficients(const cv::Mat& img, int nb_rows, int nb_cols) noexcept
    {
        cv::Mat dct_coeff = computeDCTCoeff(img);

        int dct_nb_rows = dct_coeff.rows;
        int dct_nb_cols = dct_coeff.cols;

        VecDouble coarse_coeffs;

        for (int r = 0; r < dct_nb_rows; r++)
            for (int c = 0; c < nb_cols; c++)
                coarse_coeffs.push_back(dct_coeff.at<double>(r, c));

        for (int r = 0; r < nb_rows; r++)
            for (int c = nb_cols; c < dct_nb_cols; c++)
                coarse_coeffs.push_back(dct_coeff.at<double>(r, c));

        return coarse_coeffs;
    }

    // ----------------------------------------------------------------------------
    cv::Mat DCT::computeDCTCoeff(const cv::Mat& img) noexcept
    {
        cv::Mat img_gray;

        if (img.channels() == 3)
            cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
        else if (img.channels() == 1)
            img_gray = img.clone();

        cv::Mat dct_coeff;
        if (img_gray.rows % 2 == 1 || img_gray.cols % 2 == 1)
        {
            int top = 0;
            int left = 0;
            int bottom = img_gray.rows % 2;
            int right = img_gray.cols % 2;

            cv::Mat img_tmp = img_gray.clone();
            cv::copyMakeBorder(img_tmp, img_gray, top, bottom, left, right, cv::BORDER_REFLECT101);
        }

        cv::dct(img_gray, dct_coeff);

        return dct_coeff;
    }

    // ----------------------------------------------------------------------------
    cv::Mat DCT::computeIDCTCoeff(const cv::Mat& coeffs) noexcept
    {
        cv::Mat img_64f;
        cv::idct(coeffs, img_64f);

        cv::Mat img_gray;
        img_64f.convertTo(img_gray, CV_8UC1, 255.);

        return img_gray;
    }

    // ----------------------------------------------------------------------------
    cv::Mat DCT::selectNZigZagCoefficients(const cv::Mat& coeffs, int first_coeff_num, int last_coeff_num) noexcept
    {
        cv::Mat coarse_coeffs = cv::Mat::zeros(coeffs.size(), coeffs.type());

        if (last_coeff_num == 0)
            return coarse_coeffs;

        int max_row = coeffs.rows;
        int max_col = coeffs.cols;

        int row = 0;
        int col = 0;
        int direction = 1;

        int nb_coeffs = coeffs.rows * coeffs.cols;
        last_coeff_num = std::min(nb_coeffs, last_coeff_num);

        for (int coeff_num = 0; coeff_num < last_coeff_num; coeff_num++)
        {
            if (row == 0 || col == 0 || row == max_row - 1 || col == max_col - 1)
            {
                direction *= -1;

                if (row == max_row - 1)
                    col++;
                else if (col == max_col - 1)
                    row++;
                else if (row == 0)
                    col++;
                else if (col == 0)
                    row++;

                if (coeff_num > first_coeff_num)
                    coarse_coeffs.at<double>(row, col) = coeffs.at<double>(row, col);

                coeff_num++;
            }

            row -= direction;
            col += direction;

            if (coeff_num > first_coeff_num)
                coarse_coeffs.at<double>(row, col) = coeffs.at<double>(row, col);
        }

        return coarse_coeffs;
    }

} // namespace ProjectName