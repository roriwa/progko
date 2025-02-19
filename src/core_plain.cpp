#include "core_plain.h"
#include "helper.h"

cv::Mat core_plain::convert_to_grayscale(const cv::Mat& input) {
    // create output matrix with same dimensions as input matrix but with only one value for color (gray-scale value)
    cv::Mat output(input.rows, input.cols , CV_8UC1);

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            // get pixel at [i, j] as a <B,G,R> vector
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            // extract the pixels as uchar (unsigned 8-bit) types (0..255)
            const uchar  // note: opencv format is BGR
                blue = pixel[0],
                green = pixel[1],
                red = pixel[2];
            // calculate gray-scale value
            const auto gray_value = static_cast<uchar>((0.21 * red) + (0.72 * green) + (0.07 * blue));
            // save in the output matrix
            output.at<uchar>(i, j) = {gray_value};
        }
    }

    return output;
}

cv::Mat core_plain::convert_to_hsv(const cv::Mat& input) {
    // create output matrix with same dimensions as input matrix. with three values for color (hue saturation value)
    cv::Mat output(input.rows, input.cols, CV_8UC3);

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            // get pixel at [i, j] as a <B,G,R> vector
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            // extract the pixels as uchar (unsigned 8-bit) types (0..255)
            const float  // note: opencv format is BGR
                blue = static_cast<float>(pixel[0]) / 255.0f,
                green = static_cast<float>(pixel[1]) / 255.0f,
                red = static_cast<float>(pixel[2]) / 255.0f;
            // calculate max, min and their diff of all colors
            const float cmax = max3(red, green, blue);
            const float cmin = min3(red, green, blue);
            const float diff = cmax - cmin;
            // calculate hue based on some criteria
            uchar hue;
            if (cmax == cmin) {
                hue = 0;
            } else if (cmax == red) {
                hue = static_cast<int>(60 * ((green - blue) / diff) + 360) % 180;
            } else if (cmax == green) {
                hue = static_cast<int>(60 * ((blue - red) / diff) + 120) % 180;
            } else /* if (cmax == blue) */ {
                hue = static_cast<int>(60 * ((red - green) / diff) + 240) % 180;
            }
            // calculate saturation
            const uchar saturation = (cmax == 0.0f) ? 0 : static_cast<uchar>((diff / cmax) * 255.0f);
            // calculate value
            const auto value = static_cast<uchar>(cmax * 100.0f);
            // save in the output matrix
            output.at<cv::Vec3b>(i, j) = {hue, saturation, value};
        }
    }

    return output;
}

cv::Mat core_plain::convert_to_emboss(const cv::Mat& input) {
    cv::Mat output(input.rows, input.cols , CV_8UC3);

    for (int i = 1; i < input.rows; i++) {
        for (int j = 1; j < input.cols; j++) {
            // get pixel at [i, j] as a <B,G,R> vector
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            // extract the pixels as uchar (unsigned 8-bit) types (0..255)
            const uchar  // note: opencv format is BGR
                blue = pixel[0],
                green = pixel[1],
                red = pixel[2];
            // get pixel colors of the top-left neighbor
            const auto& neighborPixel = input.at<cv::Vec3b>(i-1, j-1);
            const uchar  // note: opencv format is BGR
                tlBlue = neighborPixel[0],
                tlGreen = neighborPixel[1],
                tlRed = neighborPixel[2];
            // calculate the diff of the current colors with those of the neighbor
            const int
                diffBlue = blue - tlBlue,
                diffGreen = green - tlGreen,
                diffRed = red - tlRed;
            // calculate the maximum value while ignoring the sign
            const int max = max_ignore_sign(diffBlue, max_ignore_sign(diffGreen, diffRed));
            // calculate the gray value based off some magic while capping it between 0-255
            const uchar gray = clamp(0, 128 + max, 255);
            // save in the output matrix
            output.at<cv::Vec3b>(i, j) = {gray, gray, gray};
        }
    }

    return output;
}
