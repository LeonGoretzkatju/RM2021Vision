//
// Created by nuc on 2021/8/31.
//
#include <KalmanPosFilter.h>
#include <opencv2/opencv.hpp>
#include "log.hpp"
using namespace cv;
void KalmanPosFilter::init(cv::Point2f &target){
    cv::KalmanFilter KF(4,2,0);
    std::cout << "11" << std::endl;
    KF.transitionMatrix = (Mat_<float>(4,4) << 1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);
    std::cout << "111" << std::endl;
    //LOG(INFO) << "KalmanFilter trans: "<< KF.transitionMatrix;
    setIdentity(KF.measurementMatrix);
    //LOG(INFO) << "KalmanFilter measure: "<< KF.measurementMatrix;
    setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-4));
    setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-5));
    setIdentity(KF.errorCovPost, cv::Scalar::all(1));
    KF.statePost = (cv::Mat_<float>(4,1) << target.x,target.y,0,0);
    KF.statePre = (cv::Mat_<float>(4,1) << target.x,target.y,0,0);
}

void KalmanPosFilter::update(cv::Point2f &target){
    Mat measurement = Mat::zeros(2, 1, CV_32F);
    std::cout << "6" << std::endl;
    measurement = (cv::Mat_<float>(2,1) << target.x,target.y);
    std::cout << "64" << std::endl;
    log_msg("reached this line.")
    std::cout << "meas" << measurement << std::endl;
    KF.correct(measurement);
    std::cout << "641" << std::endl;
}

cv::Vec4f KalmanPosFilter::predict(){
    cv::Mat result = KF.predict();
    //LOG(INFO) << "KF result:" << result;
    float* data = (float*) result.data;
    return cv::Vec4f(data[0],data[1],data[2],data[3]);
}
