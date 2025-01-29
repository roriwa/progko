#include <opencv2/opencv.hpp>
#include "core_cv2.h"


cv::Mat core_cv2::convert_to_grayscale(const cv::Mat &input) {
    cv::Mat output;
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    return output;
}

cv::Mat core_cv2::convert_to_hsv(const cv::Mat &input) {
    cv::Mat output;
    cv::cvtColor(input, output, cv::COLOR_BGR2HSV);
    return output;
}

cv::Mat core_cv2::convert_to_emboss(const cv::Mat &input) {
    const cv::Mat kernel = (cv::Mat_<char>(2, 2) <<
        1, 0,
        0, -1);
    cv::Mat output;
    cv::filter2D(input, output, -1, kernel);
    output += cv::Scalar(128, 128, 128);  // Add 128 to the result to bring the values to a visible range
    return output;
}
