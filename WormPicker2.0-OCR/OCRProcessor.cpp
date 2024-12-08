#include "OCRProcessor.h"

#include <iostream>

OCRProcessor::OCRProcessor(const std::string& tessdataPath, const std::string& language)
    : tessdataPath_(tessdataPath), language_(language), image_(nullptr),
    ocrEngineMode_(1), pageSegMode_(tesseract::PSM_SPARSE_TEXT) {
    initialize();
    setOCREngineMode(ocrEngineMode_);
    setPageSegMode(pageSegMode_);
}

OCRProcessor::~OCRProcessor() {
    cleanup();
}

bool OCRProcessor::initialize() {
    // Initialize Tesseract and handle errors
    if (tess_.Init(tessdataPath_.c_str(), language_.c_str())) {
        std::cerr << "Could not initialize tesseract." << std::endl;
        return false;
    }
    return true;
}

void OCRProcessor::cleanup() {
    tess_.End();
    if (image_ != nullptr) {
        pixDestroy(&image_);
    }
}

void OCRProcessor::setOCREngineMode(int mode) {
    ocrEngineMode_ = mode;
    tess_.SetVariable("tessedit_ocr_engine_mode", std::to_string(ocrEngineMode_).c_str());
}

void OCRProcessor::printOCREngineMode() {
    std::string ocrEngineModeStr;
    if (tess_.GetVariableAsString("tessedit_ocr_engine_mode", &ocrEngineModeStr)) {
        std::cout << "Current OCR Engine Mode: " << ocrEngineModeStr << std::endl;
    }
    else {
        std::cerr << "Failed to retrieve OCR Engine Mode." << std::endl;
    }
}

void OCRProcessor::setPageSegMode(tesseract::PageSegMode mode) {
    pageSegMode_ = mode;
    tess_.SetPageSegMode(pageSegMode_);
}

void OCRProcessor::setWhitelist(const std::string& charWhitelist) {
    charWhitelist_ = charWhitelist;
    tess_.SetVariable("tessedit_char_whitelist", charWhitelist_.c_str());
}

bool OCRProcessor::loadImage(const std::string& imagePath) {
    image_ = pixRead(imagePath.c_str());
    if (image_ == nullptr) {
        std::cerr << "Could not load image: " << imagePath << std::endl;
        return false;
    }
    return true;
}

std::string OCRProcessor::performOCR(const std::string& imagePath) {
    if (!loadImage(imagePath)) {
        return "Error";
    }

    tess_.SetImage(image_);
    tess_.Recognize(0);

    std::string result = tess_.GetUTF8Text();
    pixDestroy(&image_);
    image_ = nullptr;

    return result;
}

void OCRProcessor::displayCharacterConfidence() {
    tesseract::ResultIterator* ri = tess_.GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;

    if (ri != nullptr) {
        std::cout << "Character\tConfidence" << std::endl;
        do {
            const char* symbol = ri->GetUTF8Text(level);
            float confidence = ri->Confidence(level);

            if (symbol != nullptr) {
                std::cout << symbol << "\t\t" << confidence << std::endl;
                delete[] symbol;
            }
        } while (ri->Next(level));
    }
}

void OCRProcessor::saveBoundingBoxesWithImage(const std::string& imagePath, const std::string& resultPath, tesseract::PageIteratorLevel level) {
    // Load the image using OpenCV
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return;
    }

    // Ensure that Tesseract's iterator is available
    tesseract::ResultIterator* ri = tess_.GetIterator();
    if (ri == nullptr) {
        std::cerr << "No OCR results available to display bounding boxes." << std::endl;
        return;
    }

    // Draw bounding boxes on the image
    do {
        int x1, y1, x2, y2;
        if (ri->BoundingBox(level, &x1, &y1, &x2, &y2)) {
            // Draw a rectangle for the bounding box
            cv::rectangle(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 2);

            // Optionally, add text above the rectangle
            const char* text = ri->GetUTF8Text(level);
            if (text != nullptr) {
                cv::putText(image, text, cv::Point(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1);
                delete[] text;
            }
        }
    } while (ri->Next(level));

    // Display the image with bounding boxes
    cv::imshow("Bounding Boxes", image);
    cv::waitKey(0);

    // Save the image
    cv::imwrite(resultPath, image);
}