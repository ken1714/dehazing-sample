#include "atmospheric_light.hpp"


AtmosphericLight::AtmosphericLight(const double candidateAreaRate): _candidateAreaRate(candidateAreaRate) {
    if (candidateAreaRate < 0 || candidateAreaRate > 1.0) {
        std::cerr << "Invalid candidate area rate. Please set [0, 1] value." << std::endl;
    }
}


void AtmosphericLight::execute(const cv::Mat& originalImage, const cv::Mat& darkChannelImage, cv::Vec3b& atmosphericLight){
    _candidateAreaThreshold = calculateCandidateAreaLowerbound(darkChannelImage);

    atmosphericLight = extractAtmosphericLight(originalImage, darkChannelImage, _candidateAreaThreshold);
}


cv::Mat AtmosphericLight::getCandidateAreaImage() {
    return _candidateAreaImage;
}


int AtmosphericLight::calculateCandidateAreaLowerbound(const cv::Mat& darkChannelImage) {
    const int imageHeight = darkChannelImage.rows;
    const int imageWidth  = darkChannelImage.cols;
    const int pixelNum = imageHeight * imageWidth;

    // Calculate histogram of dark channel image.
    int darkChannelHistogram[256] = {};
    for (int y = 0; y < imageHeight; y++) {
        const unsigned char* darkChannelPtr = darkChannelImage.ptr(y);
        for (int x = 0; x < imageWidth; x++) {
            darkChannelHistogram[(int)(unsigned char)darkChannelPtr[x]]++;
        }
    }

    // Get lower bound of candidate area intensity
    const int maxDarkChannel = 255;
    const double candidatePixelNum = pixelNum * _candidateAreaRate;
    int sumDarkChannel = 0;
    int candidateAreaThreshold;
    for (int intensity = maxDarkChannel; intensity >= 0; intensity--) {
        sumDarkChannel += darkChannelHistogram[intensity];
        if (sumDarkChannel >= candidatePixelNum) {
            candidateAreaThreshold = intensity;
            break;
        }
    }

    return candidateAreaThreshold;
}


cv::Vec3b AtmosphericLight::extractAtmosphericLight(const cv::Mat& originalImage, const cv::Mat& darkChannelImage, const int candidateAreaThreshold) {
    const int imageHeight = originalImage.rows;
    const int imageWidth  = originalImage.cols;

    // Extract atmospheric light from original image
    int maxIntensity = 0;
    cv::Vec3b atmosphericLight(3);
    for (int y = 0; y < imageHeight; y++) {
        const cv::Vec3b* originalImagePtr = originalImage.ptr<cv::Vec3b>(y);
        const unsigned char* darkChannelPtr = darkChannelImage.ptr(y);
        for (int x = 0; x < imageWidth; x++) {
            if ((int)(unsigned char)darkChannelPtr[x] < candidateAreaThreshold) {
                continue;
            }

            // Extract the most highest intensity as atmospheric light.
            for(int channel = 0; channel < 3; channel++) {
                int intensity = (int)(unsigned char)originalImagePtr[x][channel];
                if (maxIntensity < intensity) {
                    maxIntensity = intensity;
                    atmosphericLight = originalImagePtr[x];
                }
            }
        }
    }

    return atmosphericLight;
}


void AtmosphericLight::drawCandidateArea(const cv::Mat& originalImage, const cv::Mat& darkChannelImage, const int candidateAreaThreshold) {
    const int imageHeight = originalImage.rows;
    const int imageWidth  = originalImage.cols;

    originalImage.copyTo(_candidateAreaImage);
    for (int y = 0; y < imageHeight; y++) {
        cv::Vec3b* candidateAreaPtr = _candidateAreaImage.ptr<cv::Vec3b>(y);
        const unsigned char* darkChannelPtr = darkChannelImage.ptr(y);
        for (int x = 0; x < imageWidth; x++) {
            if ((int)(unsigned char)darkChannelPtr[x] < candidateAreaThreshold) {
                continue;
            }

            // Fill candidate area with red.
            candidateAreaPtr[x][0] = 0;
            candidateAreaPtr[x][1] = 0;
            candidateAreaPtr[x][2] = 255;
        }
    }
}
