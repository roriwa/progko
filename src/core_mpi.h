#ifndef CORE_MPI_H
#define CORE_MPI_H
#include <opencv2/core/mat.hpp>


namespace core_mpi {
    /**
     * converts an image to grayscale
     * @param srcImage input matrix
     * @return gray-scaled version of the input matrix
     */
    cv::Mat convert_to_grayscale(const cv::Mat& srcImage);

    /**
     * converts an image colors from bgr to hsv
     * @param srcImage input matrix
     * @return hsv version of the input matrix
     */
    cv::Mat convert_to_hsv(const cv::Mat& srcImage);

    /**
     * converts an image with emboss
     * @param srcImage input matrix
     * @return emboss version of the input matrix
     */
    cv::Mat convert_to_emboss(const cv::Mat& srcImage);
}

#endif //CORE_MPI_H
