#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char **argv) {
    po::positional_options_description positonalOptions;
    po::options_description allOptions("input/output options");
    po::variables_map values;
    std::string inputImageDir, outputImageDir;

    // Comanndline option
    try {
        positonalOptions.add("InputImageDir", 1)
            .add("OutputImageDir", 1);
        allOptions.add_options()
            ("help,h", "Show help")
            ("InputImageDir", po::value<std::string>(&inputImageDir)->required(), "Path to input image directory")
            ("OutputImageDir", po::value<std::string>(&outputImageDir)->required(), "Path to output image directory");

        po::store(
            po::command_line_parser{argc, argv}
                .options(allOptions)
                .positional(positonalOptions)
                .run(),
            values);

        if (values.count("help")) {
            std::cout << allOptions << std::endl;
        }
        boost::program_options::notify(values);

    } catch(po::error_with_option_name &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    // If not exist input image directory
    if (!fs::exists(inputImageDir)) {
        std::cout << "Directory " << inputImageDir << " is not found." << std::endl;
        return 1;
    }

    // If not exist output image directory
    const fs::path outputImageDirPath = fs::path(outputImageDir);
    if (!fs::exists(outputImageDir)) {
        std::cout << "Directory " << outputImageDir << " is not found, so creating it ..." << std::endl;
        boost::system::error_code error;
        const bool result = fs::create_directory(outputImageDirPath, error);
        if (!result || error) {
            std::cout << "Failed to create directory " << outputImageDir << "." << std::endl;
        } else {
            std::cout << "Succeed to create directory " << outputImageDir << "." << std::endl;
        }
    }

    // Scan all files in inputImageDir
    const fs::directory_iterator inputImageFiles= fs::directory_iterator(fs::path(inputImageDir));
    for (const fs::directory_entry& inputFile: inputImageFiles) {
        // input
        const fs::path inputFilePath = inputFile.path();
        const std::string inputFilePathStr = inputFilePath.string();
        const fs::path filename = inputFilePath.filename();
        const cv::Mat inputImage = cv::imread(inputFilePathStr);
        std::cout << filename.string() << std::endl;

        // output
        const std::string outputFilePathStr = fs::path(outputImageDirPath / filename).string();
        cv::imwrite(outputFilePathStr, inputImage);
    }

    return 0;
}
