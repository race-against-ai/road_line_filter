#ifndef CAMERAPROCESSOR_H
#define CAMERAPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

struct line {
    cv::Point start_point;
    cv::Point end_point;
    int length;

    void clear() {
        start_point = cv::Point(0, 0);
        end_point = cv::Point(0, 0);
        length = 0;
    }
};


class CameraProcessor {
public:
    CameraProcessor(const std::vector<int>& rows);
    void processStream();

private:
    cv::VideoCapture cap;
    cv::Mat frame;
    cv::Mat masked_frame;
    cv::Mat gray_frame;
    std::vector<int> rows;
    std::vector<cv::Point> white_pixels;

    int min_pixel_count_line = 5;
    int max_pixel_count_line = 25;

    int min_pixel_count_track_width = 350;
    int max_pixel_count_track_width = 500;
    
    std::vector<line> detected_lines;

    bool captureFrame();
    void maskFrame();
    void findWhitePixels();
    void displayFrame();
    void printWhitePixelPositions();
};

#endif
