//
// Created by nuc on 2021/7/23.
//
#include <iostream>
#include <math.h>
#include <fstream>
#ifndef TJU_RM_2021_DRAW_CURVE_H
#define TJU_RM_2021_DRAW_CURVE_H

using namespace std;
class DrawCurve{
public:
    void ClearSaveData();
    void InsertData(float Data1,float Data2,string s1,string s2);
};
#endif //TJU_RM_2021_DRAW_CURVE_H
