//
// Created by nuc on 2021/8/31.
//

#ifndef TJU_RM_2021_KALMANPOSFILTER_H
#define TJU_RM_2021_KALMANPOSFILTER_H
#include <opencv2/video/tracking.hpp>

class KalmanPosFilter {
public:
    cv::KalmanFilter KF;
    void init(cv::Point2f &target);
    void update(cv::Point2f &target);
    cv::Vec4f predict();
    KalmanPosFilter() {

    }
    ~KalmanPosFilter() {}
};
#endif //TJU_RM_2021_KALMANPOSFILTER_H
