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
#include "log.h"
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
    RoundQueue<Trace, 3> armor_traces;
    Eigen::Matrix<double,6,6> A, P, R, Q, H;
    Eigen::Matrix<double,6,1> x, z;
public:
    int predictor_cnt = 0;
    Filter* filter;
    EigenKalman::KalmanFilter* KF;
    DrawCurve* drawCurve;

    // TODO:
    Predictor() {
        this->KF = new EigenKalman::KalmanFilter();
        A << 1, 0, 0, 1, 0, 0,
             0, 1, 0, 0, 1, 0,
             0, 0, 1, 0, 0, 1,
             0, 0, 0, 1, 0, 0,
             0, 0, 0, 0, 1, 0,
             0, 0, 0, 0, 0, 1;

        P << 1, 0, 0, 0, 0, 0,
             0, 1, 0, 0, 0, 0,
             0, 0, 1, 0, 0, 0,
             0, 0, 0, 1, 0, 0,
             0, 0, 0, 0, 1, 0,
             0, 0, 0, 0, 0, 1;

        R << 50, 0, 0, 0, 0, 0,
             0, 50, 0, 0, 0, 0,
             0, 0, 50, 0, 0, 0,
             0, 0, 0, 50, 0, 0,
             0, 0, 0, 0, 50, 0,
             0, 0, 0, 0, 0, 50;

        Q << 0.1, 0, 0, 0, 0, 0,
             0, 0.1, 0, 0, 0, 0,
             0, 0, 0.05, 0, 0, 0,
             0, 0, 0, 0.1, 0, 0,
             0, 0, 0, 0, 0.05, 0,
             0, 0, 0, 0, 0, 0.1;

        H << 1, 0, 0, 0, 0, 0,
             0, 1, 0, 0, 0, 0,
             0, 0, 1, 0, 0, 0,
             0, 0, 0, 1, 0, 0,
             0, 0, 0, 0, 1, 0,
             0, 0, 0, 0, 0, 1;

        KF->init(6, 6, this->A, this->P, this->R, this->Q, this->H);
    }
    ~Predictor() {}

    cv::Point3f solve_pnp(Trace& trace);

    bool coordinate_trans(Trace& trace);

    bool push_back(Trace target){
        if(!coordinate_trans(target)){
            log_error("coo wrong");
        }
        armor_traces.push(target);
        return true;
    }

    Point2f predict();


    bool clear(){
        return predictor_cnt = 0 && this->armor_traces.clear();
    }
};


#endif