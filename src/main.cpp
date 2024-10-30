#include <iostream>
#include <opencv2/opencv.hpp>
#include <mpi.h>
#include <omp.h>
#include "helper.h"
#include "core.h"

std::map<std::string, cv::Mat(*)(cv::Mat)> converter_functions = {
    {"hsv", &convert_to_hsv},
    {"gray", &convert_to_grayscale},
    {"grayscale", &convert_to_grayscale},
    {"emboss", &convert_to_emboss},
};


int main(const int argc, char** argv) {
    if (argc != 3) {
        const std::vector<std::string> supported_converters = extract_keys(converter_functions);
        std::cout << "Usage: " << argv[0] << " {" << str_join(supported_converters, ",") << "} <image>" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string converter_name = argv[1];
    const auto converter = converter_functions.find(converter_name);
    if (converter == converter_functions.end()) {
        std::cout << "Unknown conversion function '" << converter_name << "'" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string input_file = argv[2];
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

    const cv::Mat output_image = converter->second(raw_image);

    const std::string output_file = add_filename_suffix(input_file, "-out");
    const bool hsv_success = cv::imwrite(output_file, output_image);
    if (!hsv_success) {
        std::cout << "Failed to save output image" << std::endl;
        return EXIT_FAILURE;
    }

    cv::imshow("Displayed Image", output_image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
