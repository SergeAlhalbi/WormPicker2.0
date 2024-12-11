#include "ImageProcessor.h"
#include "OCRProcessor.h"

#include <pybind11/embed.h>
#include <filesystem>

namespace py = pybind11;

int main() {
    // Available models
    std::vector<std::string> modelOptions = {"EasyOCR" , "TesseractOCR"};
    std::string model = modelOptions[1];

    if (model == "EasyOCR") {
        // Set environment variables for Python interpreter
        _putenv("PYTHONHOME=C:\\Users\\serge\\anaconda3");
        _putenv("PYTHONPATH=C:\\Work\\Projects\\WormPicker2.0\\WormPicker2.0-EasyOCR\\Code");

        py::scoped_interpreter guard{}; // Start the Python interpreter

        // Define the image name and paths for input and output
        bool display_result = true;
        std::string image_name = "11466.jpg"; // Image file name
        std::string input_path = "C:/Work/Projects/WormPicker2.0/WormPicker2.0-EasyOCR/Images/Raw/" + image_name;
        std::string output_path = "C:/Work/Projects/WormPicker2.0/WormPicker2.0-EasyOCR/Images/Result/res-" + image_name;

        try {
            // Import the "easyOCR" module from the "Packages" folder
            py::module_ EasyOCRModule = py::module_::import("Packages.easyOCR");

            // Access the EasyOCRProcessor class from the module
            py::object EasyOCRProcessor = EasyOCRModule.attr("EasyOCRProcessor");

            // Create an instance of EasyOCRProcessor
            py::object EasyOCR = EasyOCRProcessor(); // Call the constructor

            // Call the `perform_ocr` method and store the result (which is a Python list, or use auto)
            py::list results = EasyOCR.attr("perform_ocr")(input_path, output_path, display_result);

            // Iterate over the results and print them
            std::cout << "\nEasyOCR Results:" << std::endl;
            for (const auto& result : results) {
                // Each result is a tuple (box, text, confidence)
                py::tuple detection = result.cast<py::tuple>(); // Cast to a tuple

                py::list box = detection[0].cast<py::list>();
                std::string text = detection[1].cast<std::string>();
                double confidence = detection[2].cast<double>();

                // Print the results
                std::cout << "Text: " << text << ", Confidence: " << confidence << std::endl;
            }
            
            std::cout << "OCR completed successfully!" << std::endl;
        }
        catch (const py::error_already_set& e) {
            std::cerr << "Error calling the Python function: " << e.what() << std::endl;
            return 1;
        }

        return 0;
    }
    else if (model == "TesseractOCR") {
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
        std::filesystem::create_directories(std::filesystem::path(outputPath).parent_path());

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
        std::filesystem::create_directories(std::filesystem::path(imagePath).parent_path());
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

    std::cerr << "Model not supported!" << std::endl;
    return -1;
}