#include "ImageProcessor.h"
#include "OCRProcessor.h"

#include <iostream>

int main() {
    // Display images throught processing
    std::string imageName = "41103.jpg";
    bool showImages = false;

    // Define parameters
    cv::Rect roi(500, 50, 1500, 800);
    double resizeFactor = 2.0;
    double thresholdValue = 60.0;
    cv::Size morphKernelSize(16, 16);

    // Create an ImageProcessor instance
    ImageProcessor processor(roi, resizeFactor, thresholdValue, morphKernelSize);

    // Input and output paths
    std::string inputPath = "C:/Work/Projects/WormPicker2.0/WormPicker2.0-OCR/Images/Raw/" + imageName;
    std::string outputPath = "C:/Work/Projects/WormPicker2.0/WormPicker2.0-OCR/Images/Processed/proc-" + imageName;

    // Process the image
    processor.processImage(inputPath, outputPath, showImages);

    // Initialize the OCRProcessor with the tessdata path and language
    std::string tessdataPath = "C:/Work/Libraries/vcpkg/installed/x64-windows/share/tessdata_best";
    std::string language = "eng";
    OCRProcessor ocrProcessor(tessdataPath, language);

    // Set OCR Engine Mode and Page Segmentation Mode (similar to your old main)
    ocrProcessor.setOCREngineMode(1);
    ocrProcessor.setPageSegMode(tesseract::PSM_SPARSE_TEXT);

    // Set the character whitelist (optional)
    ocrProcessor.setWhitelist("0123456789");

    // Get and print the current OCR engine mode
    ocrProcessor.printOCREngineMode();

    // Load the image and perform OCR
    std::string imagePath = "C:/Work/Projects/WormPicker2.0/WormPicker2.0-OCR/Images/Processed/proc-" + imageName;
    std::string result = ocrProcessor.performOCR(imagePath);

    if (result != "Error") {
        // Output the OCR result
        std::cout << "OCR Result: " << std::endl;
        std::cout << result << std::endl;
    }
    else {
        std::cerr << "Error occurred while performing OCR." << std::endl;
    }

    // Get and display character confidence values
    ocrProcessor.displayCharacterConfidence();

    //Display bounding boxes
    std::string resultPath = "C:/Work/Projects/WormPicker2.0/WormPicker2.0-OCR/Images/Result/res-" + imageName;
    ocrProcessor.saveBoundingBoxesWithImage(imagePath, resultPath, tesseract::RIL_WORD);

    return 0;
}