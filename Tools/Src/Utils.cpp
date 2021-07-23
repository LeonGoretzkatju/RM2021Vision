#include "../Include/Utils.h"

double getPointLength(const cv::Point2f &p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

double get_pitch(double x, double y, double z) {
    return atan(y / sqrt(x * x + z * z));
}

double get_yaw(double x, double z) {
    if (x == 0 && z == 0)
        return 0;
    else if (x == 0 && z < 0)
        return 0;
    else if (x == 0 && z > 0)
        return 180;
    else if (z == 0 && x > 0)
        return 90;
    else if (z == 0 && x < 0)
        return -90;
    else if (x > 0 && z > 0)
        return pi - atan(x / z);
    else if (x > 0 && z < 0)
        return atan(-x / z);
    else if (x < 0 && z < 0)
        return -atan(x / z);
    else
        return -(pi - atan(-x / z));
}