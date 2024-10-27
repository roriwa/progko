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
            const int  // note: opencv format is BGR
                blue = pixel[0],
                green = pixel[1],
                red = pixel[2];
            const auto gray_value = static_cast<uchar>((0.21 * red) + (0.72 * green) + (0.07 * blue));
            output.at<uchar>(i, j) = gray_value;
        }
    }

    return output;
}

cv::Mat convert_to_hsv(cv::Mat input) {
    cv::Mat output(input.rows, input.cols, CV_8UC3);

    #pragma omp parallel for default(none) shared(input, output)
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            const float  // note: opencv format is BGR
                blue = static_cast<float>(pixel[0]) / 255.0f,
                green = static_cast<float>(pixel[1]) / 255.0f,
                red = static_cast<float>(pixel[2]) / 255.0f;
            const float cmax = std::max(red, std::max(green, blue));
            const float cmin = std::min(red, std::min(green, blue));
            const float diff = cmax - cmin;
            int hue;
            if (cmax == cmin) {
                hue = 0;
            } else if (cmax == red) {
                hue = static_cast<int>(60 * ((green - blue) / diff) + 360) % 180;
            } else if (cmax == green) {
                hue = static_cast<int>(60 * ((blue - red) / diff) + 120) % 180;
            } else /* if (cmax == blue) */ {
                hue = static_cast<int>(60 * ((red - green) / diff) + 240) % 180;
            }
            const int saturation = (cmax == 0.0f) ? 0 : static_cast<int>(diff / cmax) * 255;
            const int value = static_cast<int>(cmax * 100);
            output.at<cv::Vec3b>(i, j)[0] = hue;  // blue
            output.at<cv::Vec3b>(i, j)[1] = saturation;  // green
            output.at<cv::Vec3b>(i, j)[2] = value;  // red
        }
    }

    return output;
}