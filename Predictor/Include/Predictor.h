#ifndef __PREDICTOR_H_
#define __PREDICTOR_H_

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
};


class Predictor{
private:
    RoundQueue<Trace, 5> armor_traces;
public:
    Filter* filter;

    Predictor();
    ~Predictor();
    bool coordinate_trans(Trace& trace){
        
    }

    bool push_back(const Trace& target){
        this->armor_traces.push(target);
    }
};


#endif