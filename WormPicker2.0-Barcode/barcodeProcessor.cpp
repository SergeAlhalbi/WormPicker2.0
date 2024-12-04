#include "barcodeProcessor.h"

#include <opencv2/opencv.hpp>
#include "MultiFormatWriter.h"
#include "BitMatrix.h"
#include <ReadBarcode.h>
#include <BarcodeFormat.h>
#include <iostream>
#include <filesystem>

// Method to generate and save a barcode
void BarcodeProcessor::generateBarcode(const std::string& input, const std::string& outputPath) {
    // Barcode format
    ZXing::BarcodeFormat format = ZXing::BarcodeFormat::Code39;

    // Generate the BitMatrix for the barcode
    ZXing::BitMatrix matrix = ZXing::MultiFormatWriter(format).encode(input, 100, 60);

    // Convert BitMatrix to OpenCV Mat
    cv::Mat bitmap(matrix.height(), matrix.width(), CV_8UC1);
    for (int y = 0; y < matrix.height(); ++y) {
        for (int x = 0; x < matrix.width(); ++x) {
            bitmap.at<uchar>(y, x) = matrix.get(x, y) ? 0 : 255;
        }
    }

    // Ensure the directory exists
    std::filesystem::create_directories(std::filesystem::path(outputPath).parent_path());

    // Save the barcode image
    if (cv::imwrite(outputPath, bitmap)) {
        std::cout << "Barcode generated and saved to " << outputPath << "\n";
    }
    else {
        std::cerr << "Error: Could not save the file at " << outputPath << "\n";
    }
}

// Method to read and decode a barcode
void BarcodeProcessor::readBarcode(const std::string& imagePath) {
    // Load image using OpenCV
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

    if (img.empty()) {
        std::cerr << "Error: Unable to load image!\n";
        return;
    }

    // Convert OpenCV Mat to ZXing ImageView
    ZXing::ImageView imageView(img.data, img.cols, img.rows, ZXing::ImageFormat::Lum);

    // Set barcode reading options
    ZXing::ReaderOptions options;
    options.setFormats(ZXing::BarcodeFormat::Any); // Allow all barcode formats

    // Decode the barcode
    auto results = ZXing::ReadBarcodes(imageView, options);

    if (results.empty()) {
        std::cout << "No barcodes found in the image.\n";
    }
    else {
        for (const auto& result : results) {
            std::cout << "Barcode format: " << ZXing::ToString(result.format()) << "\n";
            std::cout << "Barcode text: " << result.text() << "\n";
        }
    }
}

// Method to show an image in an OpenCV window
void BarcodeProcessor::showImage(const std::string& imagePath) {
    cv::Mat image = cv::imread(imagePath); // Load the image
    if (image.empty()) {
        std::cout << "Could not load image!" << std::endl;
        return;
    }
    cv::imshow("Image", image); // Show the image with the window name "Image"
    cv::waitKey(0); // Wait for a key press to close the window
}