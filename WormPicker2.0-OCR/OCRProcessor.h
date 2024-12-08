#ifndef OCRPROCESSOR_H
#define OCRPROCESSOR_H

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <string>

class OCRProcessor {
public:
    OCRProcessor(const std::string& tessdataPath, const std::string& language);
    ~OCRProcessor();

    bool initialize();
    void cleanup();
    void setOCREngineMode(int mode);
    void printOCREngineMode();
    void setPageSegMode(tesseract::PageSegMode mode);
    void setWhitelist(const std::string& charWhitelist);
    bool loadImage(const std::string& imagePath);
    std::string performOCR(const std::string& imagePath);
    void displayCharacterConfidence();
    void saveBoundingBoxesWithImage(const std::string& imagePath, const std::string& resultPath, tesseract::PageIteratorLevel level);

private:
    tesseract::TessBaseAPI tess_;
    std::string tessdataPath_;
    std::string language_;
    PIX* image_;
    int ocrEngineMode_;
    tesseract::PageSegMode pageSegMode_;
    std::string charWhitelist_;
    
};

#endif // OCRPROCESSOR_H