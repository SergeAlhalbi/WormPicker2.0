#include "barcodeProcessor.h"

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

int main() {
    // Create an instance of BarcodeProcessor
    BarcodeProcessor processor;

    // List of input texts
    std::vector<std::string> inputTexts = {
        "N2", "CB4856", "JR667", "DH424",
        "MT2124", "JK1107", "VC2010", "BA671",
        "BC10555", "PS3398", "GS1910", "CL2122",
        "CB1370", "VC1134", "AU37", "LX929",
        "ZB7", "CL4176", "AQ2026", "DA609"
    };

    // Barcode image size
    const int barcodeWidth = 100;
    const int barcodeHeight = 60;

    // Padding between barcodes
    const int padding = 20;

    // Number of rows and columns for the grid layout
    const int rows = 5;
    const int cols = 4;

    // Calculate the size of the combined image including padding
    const int combinedWidth = cols * barcodeWidth + (cols + 1) * padding;
    const int combinedHeight = rows * barcodeHeight + (rows + 1) * padding;

    // Create a blank image to hold the combined barcodes with padding
    cv::Mat combinedImage = cv::Mat::zeros(combinedHeight, combinedWidth, CV_8UC1);
    combinedImage.setTo(255); // Set background to white

    // Generate and place each barcode in the combined image
    for (int i = 0; i < inputTexts.size(); ++i) {
        std::string outputPath = "barcodes/singles/barcode_" + std::to_string(i + 1) + ".png";

        // Generate the barcode
        processor.generateBarcode(inputTexts[i], outputPath);

        // Load the generated barcode image
        cv::Mat barcodeImage = cv::imread(outputPath, cv::IMREAD_GRAYSCALE);

        // Resize the barcode image if necessary
        cv::resize(barcodeImage, barcodeImage, cv::Size(barcodeWidth, barcodeHeight));

        // Calculate the position in the grid with padding
        int row = i / cols;
        int col = i % cols;
        int x = padding + col * (barcodeWidth + padding);
        int y = padding + row * (barcodeHeight + padding);
        cv::Rect roi(x, y, barcodeWidth, barcodeHeight);

        // Place the barcode image in the combined image
        barcodeImage.copyTo(combinedImage(roi));
    }

    // Save the final combined image
    std::string combinedOutputPath = "barcodes/combined/combined_barcodes.png";
    std::filesystem::create_directories(std::filesystem::path(combinedOutputPath).parent_path());
    if (cv::imwrite(combinedOutputPath, combinedImage)) {
        std::cout << "Combined barcode image saved as " << combinedOutputPath << std::endl;
    }
    else {
        std::cerr << "Error: Could not save the combined image!" << std::endl;
    }

    /*// Input text to encode
    std::string inputText = "N2";
    std::string outputPath = "barcodes/combined/combined_barcodes.png";

    // Generate and save the barcode
    processor.generateBarcode(inputText, outputPath);*/

    // Read and decode the barcode from a saved image
    std::string imagePath = "barcodes/combined/combined_barcodes.png";
    processor.readBarcode(imagePath);

    // Show the saved image
    processor.showImage(imagePath);

    return 0;
}