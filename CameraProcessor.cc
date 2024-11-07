#include "CameraProcessor.h"
#include <iostream>

CameraProcessor::CameraProcessor(const std::vector<int>& rows) 
    : rows(rows) {
    if (!cap.open("http://192.168.30.123:8443/normal.py")) {
        throw std::runtime_error("cant open camera");
    }
}

void CameraProcessor::processStream() {
    while (true) {
        if (!captureFrame()) break;
        maskFrame();
        findWhitePixels();
        displayFrame();
        printWhitePixelPositions();

        if (cv::waitKey(1) == 'q') break;
    }
    cap.release();
    cv::destroyAllWindows();
}

bool CameraProcessor::captureFrame() {
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "no image!" << std::endl;
        return false;
    }
    return true;
}

void CameraProcessor::maskFrame() {
    cv::Scalar lower_white(150, 150, 150);
    cv::Scalar upper_white(255, 255, 255);

    cv::Mat mask;
    cv::inRange(frame, lower_white, upper_white, mask);

    cv::Mat white_pixels_frame;
    cv::bitwise_and(frame, frame, white_pixels_frame, mask);

    cv::cvtColor(white_pixels_frame, gray_frame, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray_frame, gray_frame, cv::Size(5, 5), 0);

    cv::imshow("Frame", gray_frame);

    masked_frame = cv::Mat::zeros(gray_frame.size(), gray_frame.type());
    for (int row : rows) {
        if (row >= 0 && row < frame.rows) {
            gray_frame.row(row).copyTo(masked_frame.row(row));
        }
    }
}
void CameraProcessor::findWhitePixels() {
    white_pixels.clear();
    detected_lines.clear();

    for (int row : rows) {
        if (row >= 0 && row < masked_frame.rows) {
            int white_streak_length = 0;
            line pot_line;

            for (int x = 0; x < masked_frame.cols; x++) {
                uchar pixel_value = masked_frame.at<uchar>(row, x);

                if (pixel_value > 200) {
                    if (white_streak_length == 0) {
                        pot_line.start_point = cv::Point(x, row);
                    }
                    white_streak_length++;
                } else if (white_streak_length > 0) {
                    pot_line.end_point = cv::Point(x - 1, row);
                    pot_line.length = white_streak_length;
                    detected_lines.push_back(pot_line);

                    white_streak_length = 0;
                }
            }
            if (white_streak_length > 0) {
                pot_line.end_point = cv::Point(masked_frame.cols - 1, row);
                pot_line.length = white_streak_length;
                detected_lines.push_back(pot_line);
            }
        }
    }
}



void CameraProcessor::displayFrame() {
    cv::imshow("Masked Frame", masked_frame);
}


void CameraProcessor::printWhitePixelPositions() {
    cv::Mat detected_lanes = cv::Mat(frame.rows, frame.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (size_t i = 0; i < detected_lines.size(); ++i) {
        const auto& line_reference = detected_lines[i];

        if (line_reference.length > min_pixel_count_line && line_reference.length < max_pixel_count_line) {
            for (size_t j = i + 1; j < detected_lines.size(); ++j) {
                const auto& line_tester = detected_lines[j];

                if (line_tester.length > min_pixel_count_line && line_tester.length < max_pixel_count_line) {
                    int distance_x = std::abs(line_reference.start_point.x - line_tester.start_point.x);

                    if (distance_x > min_pixel_count_track_width && distance_x < max_pixel_count_track_width) {
                        cv::line(detected_lanes, line_reference.start_point, line_reference.end_point, cv::Scalar(255, 0, 0), 2);
                        cv::line(detected_lanes, line_tester.start_point, line_tester.end_point, cv::Scalar(255, 0, 0), 2);

                        std::cout << "Space between (" << i << ") und (" << j << "): " 
                                  << distance_x << " Pixel\n";
                    }
                }
            }
        }
    }

    cv::imshow("Detected Lines", detected_lanes);
}

