#include "remove_haze.hpp"


RemoveHaze::RemoveHaze(const cv::Size imageSize, const double minTransmission): ImageProcess(imageSize), _minTransmission(minTransmission) {
    if (minTransmission < 0 || minTransmission > 1.0) {
        std::cerr << "Invalid minimum value of transmission. Please set [0, 1] value." << std::endl;
    }
}


void RemoveHaze::execute(const cv::Mat& inputImage, const cv::Mat& transmissionMap, const cv::Vec3b atmosphericLight, cv::Mat& outputImage) {
    const int imageHeight = inputImage.rows;
    const int imageWidth  = inputImage.cols;
    const int channnelNum = 3;

    // Dehazing
    outputImage = cv::Mat(inputImage.size(), CV_8UC3);
    for (int y = 0; y < imageHeight; y++) {
        const cv::Vec3b* inputPtr = inputImage.ptr<cv::Vec3b>(y);
        cv::Vec3b* outputPtr = outputImage.ptr<cv::Vec3b>(y);
        const unsigned char* transmissionPtr = transmissionMap.ptr(y);
        for (int x = 0; x < imageWidth; x++) {
            const double transmission = (double)(unsigned char)transmissionPtr[x] / 255.0;
            for(int channel = 0; channel < channnelNum; channel++){
                const double inputValue = (double)(unsigned char)inputPtr[x][channel];
                const double atmosphericValue = (double)(unsigned char)atmosphericLight[channel];
                double dehazedValue = (inputValue - atmosphericValue) / std::max(transmission, _minTransmission) + atmosphericValue;
                if(dehazedValue < 0){
                    dehazedValue = 0;
                } else if(dehazedValue > 255){
                    dehazedValue = 255;
                }
                outputPtr[x][channel] = dehazedValue;
            }
        }
    }
}
