/**
 * @file opencv_helper.hpp
 * @author Jet.Sun (shuhuan.sun@phigent.ai)
 * @brief
 * @version 0.1
 *  *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef PG_UTILS_OPENCV_HELPER_HPP_
#define PG_UTILS_OPENCV_HELPER_HPP_

#include "opencv2/opencv.hpp"
#include "vision_type/base_type.hpp"
#include "vision_type/vision_type.hpp"

namespace pg {
namespace utils {
namespace helper {

int ImageFrame2CvMatBGR(phigent::vision::ImageFrame &img, cv::Mat &mat,
                        bool deep_copy);

int ImageFrame2CvMat(phigent::vision::ImageFrame &img, cv::Mat &mat,
                     bool deep_copy, bool is_dev_addr);

phigent::vision::ImageFramePtr CvMat2ImageFrame(cv::Mat &mat,
                                                PGPixelFormat format,
                                                bool deep_copy,
                                                bool is_dev_addr);

#ifdef USE_GPU
int ImageFrame2CvGpuMat(phigent::vision::ImageFrame &img, cv::cuda::GpuMat &mat,
                        bool deep_copy);

phigent::vision::ImageFramePtr CvGpuMat2ImageFrame(cv::cuda::GpuMat &mat,
                                                   PGPixelFormat format,
                                                   bool deep_copy);
#endif

}  // namespace helper

}  // namespace utils

}  // namespace pg

#endif  // !PG_UTILS_OPENCV_HELPER_HPP_
