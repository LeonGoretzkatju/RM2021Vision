#include "../Include/ArmorFinder.h"
#include "../Include/ShowImage/ShowImages.h"
#include "../../Tools/Include/systime.h"
#include <opencv2/core.hpp>
#include <iostream>
#include "Predictor.h"
#include "log.hpp"
using namespace std;

#define HOG 1
#define FIXEDWINDOW 1
#define MULTISCALE 1
#define LAB 1

ArmorFinder::ArmorFinder(const uint8_t &color, SerialManager* serial_manager,Predictor* predictor, const string &paras_folder) :
        enemy_color(color),
        state(SEARCHING_STATE),
        tracking_cnt(0),
        serial_manager(serial_manager),
        predictor(predictor),
        YawPID(1.0, 0.0003,
               0.003),  // YAW轴PID控制
        PitchPID(1.0, 0.0,
                 0.0),  // PITCH轴PID控制
        contour_area(0),
        classifier(paras_folder){
        tracker = new KCFTracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
        //log_msg("reached this line.")
        auto ivalue = cv::Point2f(0,0);
        //log_msg("reached this line.")
        ArmorPosFilter.init(ivalue);
        }

void ArmorFinder::DebugPlotInit(MainWindow *w){
        w_ = w;
    }

void ArmorFinder::run(cv::Mat &src) {
    getsystime(frame_time); //　获取当前帧时间(不是足够精确)
//    stateSearchingTarget(src);                    // for debug
//    goto end;
    VisionData vdata;
    switch (state) {
        case SEARCHING_STATE:
            // cout << "searching state" << endl;
            if (stateSearchingTarget(src)) {
                if ((target_box.armor_rect & cv::Rect2d(0, 0, 640, 480)) == target_box.armor_rect) { 
                    // 判断装甲板区域是否脱离图像区域
                    //log_msg("reached this line.")
                    
                    // TODO: 添加当前串口信息
                    tracker->init(target_box.armor_rect, src);
                    state = TRACKING_STATE;
                    tracking_cnt = 0;
                    // cout << "into track" << endl;
                    // 从串口处获取yaw pitch
                    float yaw = this->serial_manager->receive_data.curr_yaw;
                    float pitch = this->serial_manager->receive_data.curr_pitch;
                    vdata = {yaw,pitch,0,0,0,0,0};
                    serial_manager->TransformData(vdata);
                    //serial_manager->uart_send(cv::Point2f(yaw, pitch), cv::Point2f(0,0), false, true);
                }
            }
            else
            {
                float yaw = this->serial_manager->receive_data.curr_yaw;
                float pitch = this->serial_manager->receive_data.curr_pitch;
                vdata = {yaw,pitch,0,0,0,0,0};
                serial_manager->TransformData(vdata);
                //serial_manager->uart_send(cv::Point2f(yaw, pitch), cv::Point2f(0,0), false, false);
            }
            // log_msg("reached this line.")
            break;
        case TRACKING_STATE:
            // cout << "tracking state" << endl;
            if (!stateTrackingTarget(src) || ++tracking_cnt > 100) {    // 最多追踪100帧图像
                state = SEARCHING_STATE;
                predictor->clear();
                float yaw = this->serial_manager->receive_data.curr_yaw;
                float pitch = this->serial_manager->receive_data.curr_pitch;
                vdata = {yaw,pitch,0,0,0,0,0};
                serial_manager->TransformData(vdata);
                //serial_manager->uart_send(cv::Point2f(yaw, pitch), cv::Point2f(0,0), false, false);
            } else {
                // 从串口处获取yaw pitch
                double yaw = this->serial_manager->receive_data.curr_yaw;
                double pitch = this->serial_manager->receive_data.curr_pitch;
                // copy armor
                Armor target = this->target_box;

                // system time
                systime t;
                getsystime(t);
                // cout << "system time " << t << endl;
                // push
                // TODO: 处理角度
                
                Trace trace = Trace(target, t, yaw, pitch);
                if (!predictor->KF->is_Initialized)
                {
                    if (predictor->coordinate_trans(trace))
                    {
                        predictor->predictor_init(trace);
                    }
                }
                else
                {
                    predictor->push_back(trace);
                    if(predictor->predictor_cnt > 1){
                        predictVaule = predictor->predict();
                        Point2f result = cv::Point2f(predictVaule.x,this->serial_manager->receive_data.curr_pitch) ;
                        cout <<"predict yaw" << " " << predictVaule.x << endl;
                        vdata = {(float )result.x, (float )result.y,0,0,0,0,0};
                        ArmorFinder::serial_manager->TransformData(vdata);
                        //serial_manager->uart_send(result, cv::Point2f(trace.yaw,trace.pitch), false, true);
                        predictor->drawCurve->InsertData(result.x,trace.yaw,"predict value","origin value yaw");
                    } else{
                        double x = trace.world_position.x;
                        double y = trace.world_position.y;
                        double z = trace.world_position.z;
                        float yaw = get_yaw(x, z);
                        float pitch = get_pitch(x, y, z);
                        vdata = {yaw,pitch,0,0,0,0,0};
                        serial_manager->TransformData(vdata);
                        //serial_manager->uart_send(cv::Point2f(this->serial_manager->receive_data.curr_yaw, this->serial_manager->receive_data.curr_pitch), cv::Point2f(trace.yaw,trace.pitch), false, true);
                    }
                    predictor->predictor_cnt ++;
                }
            }
            break;
    }

    if(target_box.armor_rect != cv::Rect2d()){
        last_box = target_box;
    }

    if (true) {                 // 根据条件显示当前目标装甲板
        showArmorBox("box", src, target_box);
        cv::waitKey(1);
    }
}