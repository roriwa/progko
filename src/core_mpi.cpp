#include <mpi.h>
#include "core_mpi.h"
#include "helper.h"


struct ImageProperties {
    int cols;
    int rows;
    int srcRows;
    int type;
    int channels;
};


void distribute_image(const cv::Mat& srcImage, cv::Mat& partialImage, ImageProperties& props) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int image_properties[5];

    if (rank == 0) {
        image_properties[0] = srcImage.cols;
        image_properties[1] = srcImage.rows / size;
        image_properties[2] = srcImage.rows;
        image_properties[3] = srcImage.type();
        image_properties[4] = srcImage.channels();
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(image_properties, 5, MPI_INT, 0, MPI_COMM_WORLD);

    props.cols = image_properties[0];
    props.rows = image_properties[1];
    props.srcRows = image_properties[2];
    props.type = image_properties[3];
    props.channels = image_properties[4];

    partialImage = cv::Mat(props.rows, props.cols, props.type);

    MPI_Barrier(MPI_COMM_WORLD);

    const int send_size = props.rows * props.cols * props.channels;

    MPI_Scatter(
        srcImage.data, send_size, MPI_UNSIGNED_CHAR,
        partialImage.data, send_size, MPI_UNSIGNED_CHAR,
        0, MPI_COMM_WORLD
    );
}


void gather_image(cv::Mat& output, const cv::Mat& partialImage, const ImageProperties& props) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    output = cv::Mat(props.srcRows, props.cols, partialImage.type());

    MPI_Barrier(MPI_COMM_WORLD);

    const int send_size = partialImage.rows * partialImage.cols * partialImage.channels();

    MPI_Gather(
        partialImage.data, send_size, MPI_UNSIGNED_CHAR,
        output.data, send_size, MPI_UNSIGNED_CHAR,
        0, MPI_COMM_WORLD
    );
}


cv::Mat core_mpi::convert_to_grayscale(const cv::Mat& srcImage) {
    ImageProperties props{};
    cv::Mat input;
    distribute_image(srcImage, input, props);
    // create output matrix with same dimensions as input matrix but with only one value for color (gray-scale value)
    cv::Mat partialOutput(input.rows, input.cols, CV_8UC1);

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            // get pixel colors
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            const int  // note: opencv format is BGR
                blue = pixel[0],
                green = pixel[1],
                red = pixel[2];
            // calculate gray-scale value
            const auto gray_value = static_cast<uchar>((0.21 * red) + (0.72 * green) + (0.07 * blue));
            // save in the output matrix
            partialOutput.at<uchar>(i, j) = gray_value;
        }
    }

    cv::Mat output;
    gather_image(output, partialOutput, props);
    return output;
}

cv::Mat core_mpi::convert_to_hsv(const cv::Mat& srcImage) {
    ImageProperties props{};
    cv::Mat input;
    distribute_image(srcImage, input, props);
    // create output matrix with same dimensions as input matrix. with three values for color (hue saturation value)
    cv::Mat partialOutput(input.rows, input.cols, CV_8UC3);

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            // get pixel colors
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            const float  // note: opencv format is BGR
                blue = static_cast<float>(pixel[0]) / 255.0f,
                green = static_cast<float>(pixel[1]) / 255.0f,
                red = static_cast<float>(pixel[2]) / 255.0f;
            // calculate max, min and their diff of all colors
            const float cmax = max3(red, green, blue);
            const float cmin = min3(red, green, blue);
            const float diff = cmax - cmin;
            // calculate hue based on some criteria
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
            // calculate saturation
            const int saturation = (cmax == 0.0f) ? 0 : static_cast<int>(diff / cmax) * 255;
            // calculate value
            const int value = static_cast<int>(cmax * 100);
            // save in the output matrix
            partialOutput.at<cv::Vec3b>(i, j) = cv::Vec3b(hue, saturation, value);
        }
    }

    cv::Mat output;
    gather_image(output, partialOutput, props);
    return output;
}

cv::Mat core_mpi::convert_to_emboss(const cv::Mat& srcImage) {
    ImageProperties props{};
    cv::Mat input;
    distribute_image(srcImage, input, props);
    cv::Mat partialOutput(input.rows, input.cols , CV_8UC3);

    for (int i = 1; i < input.rows; i++) {
        for (int j = 1; j < input.cols; j++) {
            // get pixel colors
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            const int  // note: opencv format is BGR
                blue = pixel[0],
                green = pixel[1],
                red = pixel[2];
            // get pixel colors of the top-left neighbor
            const auto& neighborPixel = input.at<cv::Vec3b>(i-1, j-1);
            const int  // note: opencv format is BGR
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
            const int gray = clamp(0, 128 + max, 255);
            // save in the output matrix
            partialOutput.at<cv::Vec3b>(i, j) = cv::Vec3b(gray, gray, gray);
        }
    }

    cv::Mat output;
    gather_image(output, partialOutput, props);
    return output;
}
