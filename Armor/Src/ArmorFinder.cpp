#include "../Include/ArmorFinder.h"
#include "../../Tools/Include/systime.h"
#include <iostream>

using namespace std;

ArmorFinder::ArmorFinder(uint8_t &color) :
        enemy_color(color),
        state(SEARCHING_STATE),
        tracking_cnt(0) {
}

void ArmorFinder::run(cv::Mat &src) {
    getsystime(frame_time); //　获取当前帧时间(不是足够精确)
//    stateSearchingTarget(src);                    // for debug
//    goto end;
    switch (state) {
        case SEARCHING_STATE:
            if (stateSearchingTarget(src)) {
                if ((target_box.armor_rect & cv::Rect2d(0, 0, 640, 480)) == target_box.armor_rect) { // 判断装甲板区域是否脱离图像区域
                    tracker = TrackerToUse::create();                       // 成功搜寻到装甲板，创建tracker对象
                    tracker->init(src, target_box.armor_rect);
                    state = TRACKING_STATE;
                    tracking_cnt = 0;
                    cout << "into track" << endl;
                }
            }
            break;
        case TRACKING_STATE:
            if (!stateTrackingTarget(src) || ++tracking_cnt > 100) {    // 最多追踪100帧图像
                state = SEARCHING_STATE;
                cout << "into searching" << endl;
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