#ifndef __LIGHT_H_
#define __LIGHT_H_

#include <opencv2/core/core.hpp>

class Light{
public:
    cv::RotatedRect light_rect;
    double length;
    double area_ratio;
    uint8_t color;
    Light(cv::RotatedRect &r, double ratio, uint8_t color) : light_rect(r), area_ratio(ratio), color(color) {
        length = std::max(light_rect.size.height, light_rect.size.width);
    }
    Light() = default;
};

#endif