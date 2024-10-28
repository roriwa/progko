#include <iostream>
#include <opencv2/opencv.hpp>
#include <mpi.h>
#include <omp.h>
#include "helper.h"
#include "core.h"

int main(const int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <image>" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string input_file = argv[1];
    if (!file_exists(input_file)) {
        std::cout << "File '" << input_file << "' does not exist" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "File: '" << input_file << "'" << std::endl;

    const cv::Mat raw_image = imread(input_file, cv::IMREAD_COLOR);
    if (raw_image.empty()) {
        std::cout << "Failed to load input image" << std::endl;
        return EXIT_FAILURE;
    }

    const cv::Mat output_image = convert_to_emboss(raw_image);

    const std::string hsv_output_file = add_filename_suffix(input_file, "-out");
    const bool hsv_success = cv::imwrite(hsv_output_file, output_image);
    if (!hsv_success) {
        std::cout << "Failed to save output image" << std::endl;
        return EXIT_FAILURE;
    }

    cv::imshow("Displayed Image", output_image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
