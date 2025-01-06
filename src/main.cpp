#include <iostream>
#include <opencv2/opencv.hpp>
#include <mpi.h>
#include <omp.h>
#include "helper.h"
#include "core.h"

/**
 * possible converter functions to use
 */
std::map<std::string, cv::Mat(*)(cv::Mat)> converter_functions = {
    {"hsv", &convert_to_hsv},
    {"gray", &convert_to_grayscale},
    {"grayscale", &convert_to_grayscale},
    {"emboss", &convert_to_emboss},
};


int main(const int argc, char** argv) {
    // checks if the arguments are ok
    if (argc != 3) {
        const std::vector<std::string> supported_converters = extract_keys(converter_functions);
        std::cout << "Usage: " << argv[0] << " {" << str_join(supported_converters, ",") << "} <image>" << std::endl;
        return EXIT_FAILURE;
    }

    // find and get the converter function
    const std::string converter_name = argv[1];
    const auto converterEntry = converter_functions.find(converter_name);
    if (converterEntry == converter_functions.end()) {
        std::cout << "Unknown conversion function '" << converter_name << "'" << std::endl;
        return EXIT_FAILURE;
    }
    const auto converter = converterEntry->second;

    // check if the input-file exists
    const std::string input_file = argv[2];
    if (!file_exists(input_file)) {
        std::cout << "File '" << input_file << "' does not exist" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "File: '" << input_file << "'" << std::endl;

    // read input image
    const cv::Mat raw_image = imread(input_file, cv::IMREAD_COLOR);
    if (raw_image.empty()) {
        std::cout << "Failed to load input image" << std::endl;
        return EXIT_FAILURE;
    }

    // convert
    const cv::Mat output_image = converter(raw_image);

    // write input to output file
    const std::string output_file = add_filename_suffix(input_file, "-out");
    const bool hsv_success = cv::imwrite(output_file, output_image);
    if (!hsv_success) {
        std::cout << "Failed to save output image" << std::endl;
        return EXIT_FAILURE;
    }

    // display resulting image
    // todo: remove?
    cv::imshow("Displayed Image", output_image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
