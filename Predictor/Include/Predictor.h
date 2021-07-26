#ifndef __PREDICTOR_H_
#define __PREDICTOR_H_

#include <iostream>
#include "../../Tools/Include/RoundQueue.h"
#include "../../Tools/Include/systime.h"
#include "../../Armor/Include/Armor.h"
#include "../../Tools/Include/Draw_Curve.h"
#include "Filter.h"
#include <opencv2/core/core.hpp>
#include "SerialManager.h"
#include "Utils.h"
#include "KalmanFilter.h"

extern SerialManager* serial_manager;

class Trace{
public:
    Armor armor;
    systime time;
    double yaw, pitch;
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
    RoundQueue<Trace, 5> armor_traces;
public:
    Filter* filter;
    DrawCurve* drawCurve;
    EigenKalman::KalmanFilter* KF;
    Predictor() {}
    ~Predictor();

    cv::Point3f solve_pnp(Trace& trace);

    bool coordinate_trans(Trace& trace);

    bool push_back(Trace target){
        if(!coordinate_trans(target)){
            std::cout << "wrong" << std::endl;
        }
        this->armor_traces.push(target);
        double x = target.world_position.x;
        double y = target.world_position.y;
        double z = target.world_position.z;
        double yaw = get_yaw(x, z);
        double pitch = get_pitch(x, y, z);

        serial_manager->uart_send(cv::Point2f(yaw, pitch),cv::Point2f(target.yaw,target.pitch), false);
        
        std::cout << "=============================================" << endl;
        std::cout << "receive yaw: " << target.yaw << std::endl;
        std::cout << "receive pitch: " << target.pitch << std::endl;
        std::cout << "compute yaw" << yaw << std::endl;
        std::cout << "compute pitch" << pitch << std::endl; 
        std::cout << "delta yaw: " << abs(target.yaw - yaw) << endl;
        std::cout << "=============================================" << endl;
        drawCurve->InsertData(pitch,target.pitch,"after transform","origin pitch");
        return true;
    }

    bool clear(){
        return this->armor_traces.clear();
    }
};


#endif