#ifndef CORE_MPI_H
#define CORE_MPI_H
#include <opencv2/core/mat.hpp>

namespace core_mpi {
    /**
     * converts an image to grayscale
     * @param input input matrix
     * @return gray-scaled version of the input matrix
     */
    cv::Mat convert_to_grayscale(const cv::Mat& input);

    /**
     * converts an image colors from bgr to hsv
     * @param input input matrix
     * @return hsv version of the input matrix
     */
    cv::Mat convert_to_hsv(const cv::Mat& input);

    /**
     * converts an image with emboss
     * @param input input matrix
     * @return emboss version of the input matrix
     */
    cv::Mat convert_to_emboss(const cv::Mat& input);
}

#endif //CORE_MPI_H
