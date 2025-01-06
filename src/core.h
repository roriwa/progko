//
// Created by playerg9 on 27.10.24.
//

#ifndef CORE_H
#define CORE_H
#include <opencv2/core/mat.hpp>

/**
 * converts an image to grayscale
 * @param input input matrix
 * @return gray-scaled version of the input matrix
 */
cv::Mat convert_to_grayscale(cv::Mat input);

/**
 * converts an image colors from bgr to hsv
 * @param input input matrix
 * @return hsv version of the input matrix
 */
cv::Mat convert_to_hsv(cv::Mat input);

/**
 * converts an image with emboss
 * @param input input matrix
 * @return emboss version of the input matrix
 */
cv::Mat convert_to_emboss(cv::Mat input);

#endif //CORE_H
