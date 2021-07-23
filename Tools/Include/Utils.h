#ifndef __UTILL_H_
#define __UTILL_H_

#include <opencv2/core/core.hpp>
#include <cmath>

const double pi = CV_PI;

double getPointLength(const cv::Point2f &p);

double get_pitch(float x, float y, float z);
double get_yaw(float x, float z);

#endif