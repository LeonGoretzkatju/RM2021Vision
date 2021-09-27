#ifndef __PREDICTOR_H_
#define __PREDICTOR_H_

#include <iostream>
#include "../../Tools/Include/RoundQueue.h"
#include "../../Tools/Include/systime.h"
#include "../../Armor/Include/Armor.h"
#include "Filter.h"
#include <opencv2/core/core.hpp>
#include "SerialManager.h"
#include "Utils.h"
#include "KalmanFilter.h"
#include "Eigen/Dense"
#include "../Include/Draw_Curve.h"

extern SerialManager* serial_manager;

class Trace{
public:
    Armor armor;
    systime time;
    double yaw, pitch;
    float distance;
    
    cv::Point3f world_position;
    Trace() {}
    Trace(Armor armor, systime time, double yaw, double pitch){
        this->armor = armor;
        this->time = time;
        this->yaw = yaw;
        this->pitch = pitch;
    }
};

class Predictor{
private:
    RoundQueue<Trace, 2> armor_traces;
    Eigen::Matrix<double,4,4> A, P, Q;
    Eigen::Matrix<double,4,4> H;
    Eigen::Matrix<double,4,4> R;
public:
    int predictor_cnt = 0;
    Filter* filter;
    EigenKalman::KalmanFilter* KF;
    DrawCurve* drawCurve;
    double last_yaw_speed;
    double last_pitch_speed;
    double last_time_stamp;

    // TODO:
    Predictor() {
        this->KF = new EigenKalman::KalmanFilter();
        A << 1, 0, 1, 0,
             0, 1, 0, 1,
             0, 0, 1, 0,
             0, 0, 0, 1;

        P << 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1;

        R << 1500, 0, 0, 0,
             0, 1500, 0, 0,
             0, 0, 1700, 0,
             0, 0, 0, 1700;

        Q << 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1,


        H << 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1;

        KF->init(4, 4, this->A, this->P, this->R, this->Q, this->H);
    }
    ~Predictor() {}

    cv::Point3f solve_pnp(Trace& trace);

    bool coordinate_trans(Trace& trace);

    bool push_back(Trace target){
        if(!coordinate_trans(target)){
            // log_error("coo wrong");
        }
        armor_traces.push(target);
        return true;
    }

    void predictor_init(Trace &target);

    Point2f predict();


    bool clear(){
        return predictor_cnt = 0 && this->armor_traces.clear();
    }
};


#endif