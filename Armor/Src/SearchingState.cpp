#include "../Include/ArmorFinder.h"

bool ArmorFinder::stateSearchingTarget(cv::Mat &src) {
    if (findArmors(src, target_box)) { // 在原图中寻找目标，并返回是否找到
        if (last_box.armor_rect != cv::Rect2d() &&
            (getPointLength(last_box.getCenter() - target_box.getCenter()) > last_box.armor_rect.height * 2.0) &&
            anti_switch_cnt++ < 3) { // 判断当前目标和上次有效目标是否为同一个目标
            target_box = Armor(); // 并给３帧的时间，试图找到相同目标
            // LOGM("anti-switch!");    // 即刚发生目标切换内的３帧内不发送目标位置
            cout << "anti-switch" << endl;
            return false;            // 可以一定程度避免频繁多目标切换
        } else {
            anti_switch_cnt = 0;
            return true;
        }
    } else {
        target_box = Armor();
        anti_switch_cnt++;
        return false;
    }
}