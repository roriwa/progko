#include <iostream>
#include <opencv2/opencv.hpp>
#include <mpi.h>
#include <omp.h>
#include "helper.h"
#include "core_plain.h"
#include "core_omp.h"
#include "core_mpi.h"
#include "core_omp_mpi.h"

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
    {"omp-mpi-emboss", &core_omp_mpi::convert_to_emboss},
    {"omp-mpi-hsv", &core_omp_mpi::convert_to_hsv},
    {"omp-mpi-grayscale", &core_omp_mpi::convert_to_grayscale},
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
        printf("Usage: %s {%s} <image>\n", argv[0], str_join(supported_converters, ",").c_str());
        return EXIT_FAILURE;
    }

    // find and get the converter function
    const std::string converter_name = argv[1];
    const auto converterEntry = converter_functions.find(converter_name);
    if (converterEntry == converter_functions.end()) {
        printf("Unknown conversion function '%s'\n", converter_name.c_str());
        return EXIT_FAILURE;
    }
    const auto converter = converterEntry->second;

    // check if the input-file exists
    const std::string input_file = argv[2];
    if (!file_exists(input_file)) {
        printf("File '%s' does not exist\n", input_file.c_str());
        return EXIT_FAILURE;
    }
    if (rank == 0)
        printf("File: '%s'\n", input_file.c_str());

    cv::Mat raw_image;

    if (rank == 0) {
        // read input image
        raw_image = cv::imread(input_file, cv::IMREAD_COLOR);
        if (raw_image.empty()) {
            printf("Failed to load input image\n");
            return EXIT_FAILURE;
        }
    }

    // convert
    const double time_start = omp_get_wtime();
    const cv::Mat output_image = converter(raw_image);
    const double time_end = omp_get_wtime();
    const double time_taken = time_end - time_start;
    if (rank == 0)
        printf("Converter function took %f seconds\n", time_taken);

    if (rank == 0) {
        // write input to output file
        const std::string output_file = add_filename_suffix(input_file, "-out");
        const bool hsv_success = cv::imwrite(output_file, output_image);
        if (!hsv_success) {
            printf("Failed to save output image\n");
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
