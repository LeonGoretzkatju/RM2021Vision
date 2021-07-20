#include "../Include/Armor.h"

Armor::Armor(const cv::Rect &pos, const Lights &blobs, uint8_t color, int i) :
        armor_rect(pos), lights(blobs), box_color(color), id(i) {};

// 获取装甲板中心点
cv::Point2f Armor::getCenter() const {
    return cv::Point2f(
            armor_rect.x + armor_rect.width / 2,
            armor_rect.y + armor_rect.height / 2
    );
}

// 获取两个灯条中心点的间距
double Armor::getBlobsDistance() const {
    if (lights.size() == 2) {
        auto &x = lights[0].light_rect.center;
        auto &y = lights[1].light_rect.center;
        return sqrt((x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y));
    } else {
        return 0;
    }
}

// 获取灯条长度和间距的比例
double Armor::lengthDistanceRatio() const {
    if (lights.size() == 2) {
        return std::max(lights[0].length, lights[1].length)
               / getBlobsDistance();
    } else {
        return 100;
    }
}

double Armor::get_real_heigt() const {
    // TODO: add other armors' height.
    return 125;
}

double Armor::get_real_width() const {
    // TODO:
    return 135;
}

// 装甲板的优先级比较
bool Armor::operator<(const Armor &box) const {
    return 0;
}
