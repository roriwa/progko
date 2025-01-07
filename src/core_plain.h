#ifndef CORE_PLAIN_H
#define CORE_PLAIN_H
#include <opencv2/opencv.hpp>

namespace core_plain {
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
}


#endif //CORE_PLAIN_H
