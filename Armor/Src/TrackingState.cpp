#include "../Include/ArmorFinder.h"
#include <iostream>
#define HOG 1
#define FIXEDWINDOW 1
#define MULTISCALE 1
#define LAB 1
bool ArmorFinder::stateTrackingTarget(cv::Mat &src) {
    cv::Rect2d pos = target_box.armor_rect;
    // if(tracker->update(src) == cv::Rect()){
    //     target_box = Armor();
    //     // LOGW("Track fail!");
    //     return false;
    // }
    if((pos & cv::Rect2d(0, 0, 640, 480)) != pos){
        target_box = Armor();
        // LOGW("Track out range!");
        return false;
    }

    // 获取相较于追踪区域两倍长款的区域，用于重新搜索，获取灯条信息
    cv::Rect2d bigger_rect;
    bigger_rect.x = pos.x - pos.width / 2.0;
    bigger_rect.y = pos.y - pos.height / 2.0;
    bigger_rect.height = pos.height * 2;
    bigger_rect.width  = pos.width * 2;
    bigger_rect &= cv::Rect2d(0, 0, 640, 480);
    cv::Mat roi = src(bigger_rect).clone();

    Armor box;
    // 在区域内重新搜索。
    if(findArmors(roi, box)) { // 如果成功获取目标，则利用搜索区域重新更新追踪器
        target_box = box;
        target_box.armor_rect.x += bigger_rect.x; //　添加roi偏移量
        target_box.armor_rect.y += bigger_rect.y;
        for(auto &blob : target_box.lights){
            blob.light_rect.center.x += bigger_rect.x;
            blob.light_rect.center.y += bigger_rect.y;
        }
        // std::cout << target_box.getCenter().x << " " << target_box.getCenter().y << std::endl;
        // tracker = TrackerToUse::create();
        tracker = new KCFTracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
        tracker->init(target_box.armor_rect,src);
    }else{    // 如果没有成功搜索目标，则使用判断是否跟丢。
        roi = src(pos).clone();
        if(classifier){ // 分类器可用，使用分类器判断。
            cv::resize(roi, roi, cv::Size(48, 36));
            if(classifier(roi) == 0){
                target_box = Armor();
                // LOGW("Track classify fail range!");
                return false;
            }
        }else{ //　分类器不可用，使用常规方法判断
            cv::Mat roi_gray;
            cv::cvtColor(roi, roi_gray, COLOR_RGB2GRAY);
            cv::threshold(roi_gray, roi_gray, 180, 255, cv::THRESH_BINARY);
            contour_area = cv::countNonZero(roi_gray);
            if(abs(cv::countNonZero(roi_gray) - contour_area) > contour_area * 0.3){
                target_box = Armor();
                return false;
            }
        }
        target_box.armor_rect = pos;
        target_box.lights.clear();
    }
    return true;
}