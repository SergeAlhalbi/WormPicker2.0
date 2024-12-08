#include "ImageProcessor.h"
#include <iostream>

ImageProcessor::ImageProcessor(const cv::Rect& roi, double resizeFactor, double thresholdValue, const cv::Size& morphKernelSize)
    : roi_(roi), resizeFactor_(resizeFactor), thresholdValue_(thresholdValue), morphKernelSize_(morphKernelSize) {
}

void ImageProcessor::processImage(const std::string& inputPath, const std::string& outputPath, bool showImages) const {
    // Read the image in grayscale
    cv::Mat img = cv::imread(inputPath, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Could not load image: " << inputPath << std::endl;
        return;
    }
    displayImage("Original Image", img, showImages);

    // Crop the image
    cv::Mat cropped_img = img(roi_);
    displayImage("Cropped Image", cropped_img, showImages);

    // Resize to simulate higher DPI
    cv::Mat resized_img;
    cv::resize(cropped_img, resized_img, cv::Size(), resizeFactor_, resizeFactor_, cv::INTER_CUBIC);
    displayImage("Resized Image", resized_img, showImages);

    // Apply binary threshold
    cv::Mat thresh_img;
    cv::threshold(resized_img, thresh_img, thresholdValue_, 255, cv::THRESH_BINARY);
    displayImage("Threshold Image", thresh_img, showImages);

    // Invert the binary image
    cv::Mat inverted;
    cv::bitwise_not(thresh_img, inverted);
    displayImage("Inverted Image", inverted, showImages);

    // Perform connected components analysis
    cv::Mat labels, stats, centroids;
    int num_components = cv::connectedComponentsWithStats(inverted, labels, stats, centroids);

    // Identify the largest connected component
    int largest_label = findLargestComponent(num_components, stats);

    // Highlight the largest component
    cv::Mat largest_component = (labels == largest_label);
    thresh_img.setTo(255, largest_component);
    displayImage("Largest Component Image", thresh_img, showImages);

    // Apply morphological operations
    cv::Mat morph_img = applyMorphology(thresh_img);
    displayImage("Morphological Image", morph_img, showImages);

    // Save the processed image
    saveImage(morph_img, outputPath);

    if (showImages) {
        // Close all OpenCV windows
        cv::destroyAllWindows();
    }
}

void ImageProcessor::displayImage(const std::string& windowName, const cv::Mat& image, bool showImages) const {
    if (showImages) {
        cv::imshow(windowName, image);
        cv::waitKey(0);
    }
}

int ImageProcessor::findLargestComponent(int numComponents, const cv::Mat& stats) const {
    int largest_area = 0, largest_label = 0;
    for (int i = 1; i < numComponents; ++i) { // Skip the background (label 0)
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area > largest_area) {
            largest_area = area;
            largest_label = i;
        }
    }
    return largest_label;
}

cv::Mat ImageProcessor::applyMorphology(const cv::Mat& img) const {
    cv::Mat morph_img;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, morphKernelSize_);
    cv::dilate(img, morph_img, kernel);
    cv::erode(morph_img, morph_img, kernel);
    return morph_img;
}

void ImageProcessor::saveImage(const cv::Mat& image, const std::string& outputPath) const {
    if (!cv::imwrite(outputPath, image)) {
        std::cerr << "Could not save the processed image to: " << outputPath << std::endl;
    }
    else {
        std::cout << "Processed image saved to: " << outputPath << std::endl;
    }
}