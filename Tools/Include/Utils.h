#ifndef __UTILL_H_
#define __UTILL_H_

#include <opencv2/core/core.hpp>
#include <cmath>

const double pi = CV_PI;

double getPointLength(const cv::Point2f &p);

double get_pitch(double x, double y, double z);
double get_yaw(double x, double z);

#endif