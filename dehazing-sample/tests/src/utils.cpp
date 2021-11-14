#include "utils.hpp"

bool equal2Images(const cv::Mat& image1, const cv::Mat& image2) {
    // Compare 2 images (If 2 images are same, equalFlag will be set 255 to all pixels.)
    cv::Mat equalFlag;
    cv::compare(image1, image2, equalFlag, cv::CMP_EQ);

    // Check all equalFlag value is the same value.
    double minValue, maxValue;
    cv::minMaxLoc(equalFlag, &minValue, &maxValue);

    // If image1 and image2 are same, minValue will be equal to maxValue.
    if (fabs(maxValue - minValue) < DBL_EPSILON) {
        return true;
    } else {
        return false;
    }
}
