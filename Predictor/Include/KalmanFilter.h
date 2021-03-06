//
// Created by nuc on 2021/7/25.
//

#ifndef TJU_RM_2021_KALMANFILTER_H
#define TJU_RM_2021_KALMANFILTER_H
#include "opencv2/opencv.hpp"
#include "Eigen/Dense"
namespace EigenKalman{
    class KalmanFilter
    {
    private:
        int stateSize;     //状态方程维数
        int measSize;      //测量参数个数
        Eigen::MatrixXd A; //状态转移矩阵
        Eigen::MatrixXd P; //协方差矩阵
        Eigen::MatrixXd H; //测量矩阵
        Eigen::MatrixXd R; //测量误差
        Eigen::MatrixXd Q; //预测误差
    public:
        KalmanFilter();
        ~KalmanFilter(){}

        bool is_Initialized = false;
        bool  first_set = true;
        double time_stamp;
        Eigen::Matrix<double,4,1> statePose;
        Eigen::Matrix<double,4,1> measPose;
        Eigen::Matrix<double,4,2> B;
        Eigen::Matrix<double,2,1> u;
        /**
        * @函数: init
        * @描述: 初始化kalman预判参数
         */
        void init(int state, int means, Eigen::Matrix<double, 4, 4>& _A, Eigen::Matrix<double, 4, 4>& _P, Eigen::Matrix<double, 4, 4>& _R, Eigen::Matrix<double, 4, 4>& _Q, Eigen::Matrix<double, 4, 4>& _H);
        /**
        * @函数: predict
        * @描述: kalman预测
        * @输入输出:Eigen::VectorXd &x 状态向量
        */
        void predict();
        /**
        * @函数: update
        * @描述: kalman更新
        * @输入: Eigen::VectorXd z_meas 测量参数向量
        * @输入输出:Eigen::VectorXd &x 状态向量
         */
        void update();
    };
}



#endif //TJU_RM_2021_KALMANFILTER_H
