//
// Created by playerg9 on 27.10.24.
//

#include "core.h"

cv::Mat convert_to_grayscale(cv::Mat input) {
    cv::Mat output(input.rows, input.cols , CV_8UC1);

    #pragma omp parallel for default(none) shared(input, output)
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            const int red = pixel[2], blue = pixel[1], green = pixel[0];  // note: opencv format is BGR
            const auto gray_value = static_cast<uchar>((0.21 * red) + (0.72 * green) + (0.07 * blue));
            output.at<uchar>(i, j) = gray_value;
        }
    }

    return output;
}
