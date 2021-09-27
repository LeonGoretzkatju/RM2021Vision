//
// Created by nuc on 2021/7/25.
//
#include "KalmanFilter.h"
namespace EigenKalman {

    KalmanFilter::KalmanFilter() {
    }

    void KalmanFilter::init(int state, int meas,
                            Eigen::Matrix<double, 4, 4> & _A, Eigen::Matrix<double, 4, 4> & _P,
                            Eigen::Matrix<double, 4, 4> & _R, Eigen::Matrix<double, 4, 4> & _Q,
                            Eigen::Matrix<double, 4, 4> & _H){
        stateSize = state;
        measSize = meas;
        A = _A;
        P = _P;
        R = _R;
        Q = _Q;
        H = _H;
    }

    void KalmanFilter::predict(){
        statePose = A * statePose + B * u;
        Eigen::MatrixXd A_T = A.transpose();
        P = A * P*A_T + Q;
    }

    void KalmanFilter::update(){
        Eigen::MatrixXd temp1, temp2, Ht;
        Ht = H.transpose();
        temp1 = H * P * Ht + R;
        temp2 = temp1.inverse();//(H*P*H'+R)^(-1)
        Eigen::MatrixXd K = P * Ht*temp2;
        Eigen::VectorXd z = H * statePose;
        statePose = statePose + K * (measPose - z);
        Eigen::MatrixXd I = Eigen::MatrixXd::Identity(stateSize, stateSize);
        P = (I - K * H)*P;
    }
}
