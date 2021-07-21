#ifndef __PREDICTOR_H_
#define __PREDICTOR_H_

#include <iostream>
#include "../../Tools/Include/RoundQueue.h"
#include "../../Tools/Include/systime.h"
#include "../../Armor/Include/Armor.h"
#include "Filter.h"
#include <opencv2/core/core.hpp>

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

    Predictor() {}
    ~Predictor();

    cv::Point3f solve_pnp(Trace& trace);

    bool coordinate_trans(Trace& trace);

    bool push_back(Trace target){
        if(!coordinate_trans(target)){
            std::cout << "wrong" << std::endl;
        }
        this->armor_traces.push(target);
        return true;
    }

    bool clear(){
        return this->armor_traces.clear();
    }
};


#endif