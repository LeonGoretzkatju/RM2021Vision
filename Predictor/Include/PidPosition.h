//
// Created by nuc on 2021/9/1.
//

#ifndef TJU_RM_2021_PIDPOSITION_H
#define TJU_RM_2021_PIDPOSITION_H
class PidPosition{
public:
    double KP,KI,KD;
    double sum_max,output_max;
    double output;
    double sum;
    double last_error;
    PidPosition(double _KP = 0.8,double _KI = 0.01,double _KD = 0.4,double _sum_max = 1024,double _output_max = 1024);
    double updateError(double error);
    void clear();
};
#endif //TJU_RM_2021_PIDPOSITION_H
