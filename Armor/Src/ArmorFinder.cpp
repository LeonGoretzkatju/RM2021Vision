#include "../Include/ArmorFinder.h"
#include "../../Tools/Include/systime.h"
#include <opencv2/core.hpp>
#include <iostream>
#include "Predictor.h"
#include "log.h"

using namespace std;

ArmorFinder::ArmorFinder(const uint8_t &color, SerialManager* serial_manager, Predictor* predictor, const string &paras_folder) :
        enemy_color(color),
        state(SEARCHING_STATE),
        tracking_cnt(0),
        serial_manager(serial_manager),
        predictor(predictor),
        classifier(paras_folder)
        {}

void ArmorFinder::run(cv::Mat &src) {
    getsystime(frame_time); //　获取当前帧时间(不是足够精确)
//    stateSearchingTarget(src);                    // for debug
//    goto end;
    switch (state) {
        case SEARCHING_STATE:
            cout << "searching state" << endl;
            if (stateSearchingTarget(src)) {
                if ((target_box.armor_rect & cv::Rect2d(0, 0, 640, 480)) == target_box.armor_rect) { 
                    // 判断装甲板区域是否脱离图像区域
                    
                    // TODO: 添加当前串口信息
                    state = TRACKING_STATE;
                    tracking_cnt = 0;
                    // cout << "into track" << endl;
                    // 从串口处获取yaw pitch
                    double yaw = this->serial_manager->receive_data.curr_yaw;
                    double pitch = this->serial_manager->receive_data.curr_pitch;
                    serial_manager->uart_send(cv::Point2f(yaw, pitch), cv::Point2f(0,0), false);
                }
            }
            break;
        case TRACKING_STATE:
            cout << "tracking state" << endl;
            if (!stateTrackingTarget(src) || ++tracking_cnt > 200) {    // 最多追踪100帧图像
                state = SEARCHING_STATE;
                cout << "if" << endl;
                // cout << "into searching" << endl;
                predictor->clear();
            } else {
                // 从串口处获取yaw pitch
                double yaw = this->serial_manager->receive_data.curr_yaw;
                double pitch = this->serial_manager->receive_data.curr_pitch;
                // copy armor
                Armor target = this->target_box;

                // system time
                systime t;
                getsystime(t);
                cout << "system time " << t << endl;
                // push
                // TODO: 处理角度
//                drawCurve.InsertData();
                Trace trace = Trace(target, t, yaw, pitch);
                predictor->push_back(trace);

                if(predictor->predictor_cnt > 2){
                    Point2f result = predictor->predict();
                    serial_manager->uart_send(result, cv::Point2f(trace.yaw,trace.pitch), false);
                    predictor->drawCurve->InsertData(result.x,trace.yaw,"predict value","origin value yaw");
                } else{
//                    double x = trace.world_position.x;
//                    double y = trace.world_position.y;
//                    double z = trace.world_position.z;
//                    double yaw = get_yaw(x, z);
//                    double pitch = get_pitch(x, y, z);
//                    drawCurve->InsertData(pitch,trace.pitch,"after transform","origin pitch");
                    serial_manager->uart_send(cv::Point2f(this->serial_manager->receive_data.curr_yaw, this->serial_manager->receive_data.curr_pitch), cv::Point2f(trace.yaw,trace.pitch), false);
                }
                predictor->predictor_cnt ++;
            }
            break;
    }

    if(target_box.armor_rect != cv::Rect2d()){
        last_box = target_box;
    }

    // if (show_armor_box) {                 // 根据条件显示当前目标装甲板
    //     showArmorBox("box", src, target_box);
    //     cv::waitKey(1);
    // }
}