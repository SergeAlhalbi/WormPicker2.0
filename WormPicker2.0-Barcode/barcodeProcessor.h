#ifndef BARCODEPROCESSOR_H
#define BARCODEPROCESSOR_H

#include <string>

class BarcodeProcessor {
public:
    // Method to generate and save a barcode
    void generateBarcode(const std::string& input, const std::string& outputPath);

    // Method to read and decode a barcode
    void readBarcode(const std::string& imagePath);

    // Method to show an image in an OpenCV window
    void showImage(const std::string& imagePath);
};

#endif // BARCODEPROCESSOR_H