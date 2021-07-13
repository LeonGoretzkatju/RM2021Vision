#ifndef __UTILL_H_
#define __UTILL_H_

double getPointLength(const cv::Point2f &p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

#endif