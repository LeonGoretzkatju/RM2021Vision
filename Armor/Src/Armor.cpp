#include "../Include/Armor.h"
#include "../../Tools/Include/constants.h"
#include <map>
#include <string>

std::map<int, std::string> id2name = {                               //装甲板id到名称的map
        {-1, "OO"},{ 0, "NO"},
        { 1, "B1"},{ 2, "B2"},{ 3, "B3"},{ 4, "B4"},{ 5, "B5"},{ 6, "B7"},{ 7, "B8"},
        { 8, "R1"},{ 9, "R2"},{10, "R3"},{11, "R4"},{12, "R5"},{13, "R7"},{14, "R8"},
};

std::map<std::string, int> name2id = {                               //装甲板名称到id的map
        {"OO", -1},{"NO",  0},
        {"B1",  1},{"B2",  2},{"B3",  3},{"B4",  4},{"B5",  5},{"B7",  6},{"B8",  7},
        {"R1",  8},{"R2",  9},{"R3", 10},{"R4", 11},{"R5", 12},{"R7", 13},{"R8", 14},
};

std::map<std::string, int> prior_blue = {
        {"B8", 0}, {"B1", 1}, {"B3", 2}, {"B4", 2}, {"B5", 2}, {"B7", 3}, {"B2", 4},
        {"R8", 5}, {"R1", 6}, {"R3", 7}, {"R4", 7}, {"R5", 7}, {"R7", 8}, {"R2", 9},
        {"NO", 10},
};

std::map<std::string, int> prior_red = {
        {"R8", 0}, {"R1", 1}, {"R3", 2}, {"R4", 2}, {"R5", 2}, {"R7", 3}, {"R2", 4},
        {"B8", 5}, {"B1", 6}, {"B3", 7}, {"B4", 7}, {"B5", 7}, {"B7", 8}, {"B2", 9},
        {"NO", 10},
};

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
    return 55;
}

double Armor::get_real_width() const {
    // TODO:
    return 230;
}

// 装甲板的优先级比较
bool Armor::operator<(const Armor &box) const {
    if (id != box.id) {
        if (box_color == BOX_BLUE) {
            return prior_blue[id2name[id]] < prior_blue[id2name[box.id]];
        } else {
            return prior_red[id2name[id]] < prior_red[id2name[box.id]];
        }
    } else {
        auto d1 = (armor_rect.x - IMAGE_CENTER_X) * (armor_rect.x - IMAGE_CENTER_X)
                  + (armor_rect.y - IMAGE_CENTER_Y) * (armor_rect.y - IMAGE_CENTER_Y);
        auto d2 = (box.armor_rect.x - IMAGE_CENTER_X) * (box.armor_rect.x - IMAGE_CENTER_X)
                  + (box.armor_rect.y - IMAGE_CENTER_Y) * (box.armor_rect.y - IMAGE_CENTER_Y);
        return d1 < d2;
    }
    return 0;
}
