//
// Created by playerg9 on 27.10.24.
//

#ifndef CORE_H
#define CORE_H
#include <opencv2/core/mat.hpp>

cv::Mat convert_to_grayscale(cv::Mat input);

cv::Mat convert_to_hsv(cv::Mat input);

cv::Mat convert_to_emboss(cv::Mat input);

#endif //CORE_H
