#include <iostream>
#include <opencv2/opencv.hpp>
#include <mpi.h>
#include "helper.h"
#include "core_plain.h"
#include "core_omp.h"
#include "core_mpi.h"

/**
 * possible converter functions to use
 */
std::map<std::string, cv::Mat(*)(const cv::Mat&)> converter_functions = {
    {"plain-hsv", &core_plain::convert_to_hsv},
    {"plain-grayscale", &core_plain::convert_to_grayscale},
    {"plain-emboss", &core_plain::convert_to_emboss},
    {"omp-hsv", &core_omp::convert_to_hsv},
    {"omp-grayscale", &core_omp::convert_to_grayscale},
    {"omp-emboss", &core_omp::convert_to_emboss},
    {"mpi-hsv", &core_mpi::convert_to_hsv},
    {"mpi-grayscale", &core_mpi::convert_to_grayscale},
    {"mpi-emboss", &core_mpi::convert_to_emboss},
};


int main(const int argc, char** argv) {
    // int rank, size, provided;
    // MPI_Init_thread(nullptr, nullptr, MPI_THREAD_FUNNELED, &provided);
    int rank, size;
    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

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
    if (rank == 0)
        std::cout << "File: '" << input_file << "'" << std::endl;

    cv::Mat raw_image;

    if (rank == 0) {
        // read input image
        raw_image = cv::imread(input_file, cv::IMREAD_COLOR);
        if (raw_image.empty()) {
            std::cout << "Failed to load input image" << std::endl;
            return EXIT_FAILURE;
        }
    }

    // convert
    const cv::Mat output_image = converter(raw_image);

    if (rank == 0) {
        // write input to output file
        const std::string output_file = add_filename_suffix(input_file, "-out");
        const bool hsv_success = cv::imwrite(output_file, output_image);
        if (!hsv_success) {
            std::cout << "Failed to save output image" << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (rank == 0 && std::getenv("NO_SHOW") == nullptr) {
        cv::imshow(input_file, output_image);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
