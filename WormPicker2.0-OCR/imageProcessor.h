#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessor {
public:
    ImageProcessor(const cv::Rect& roi, double resizeFactor, double thresholdValue, const cv::Size& morphKernelSize);

    void processImage(const std::string& inputPath, const std::string& outputPath, bool showImages) const;

private:
    bool showImages = false;
    cv::Rect roi_;
    double resizeFactor_;
    double thresholdValue_;
    cv::Size morphKernelSize_;

    void displayImage(const std::string& windowName, const cv::Mat& image, bool showImages) const;
    int findLargestComponent(int numComponents, const cv::Mat& stats) const;
    cv::Mat applyMorphology(const cv::Mat& img) const;
    void saveImage(const cv::Mat& image, const std::string& outputPath) const;
};