#include "CameraProcessor.h"
#include <iostream>

int main() {
    try {
        std::vector<int> rows = {480, 460, 440, 420, 400, 360, 320, 280, 240};

        for (int i = 480; i >= 300; i -= 1) {
            rows.push_back(i);
        }
        
        CameraProcessor camera_processor(rows);
        camera_processor.processStream();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
