#ifndef __ARMOR_H_
#define __ARMOR_H_

#include "Light.h"
#include <vector>

// extern std::map<int, std::string> id2name;   //装甲板id到名称的map
// extern std::map<std::string, int> name2id;   //装甲板名称到id的map
// extern std::map<std::string, int> prior_blue;
// extern std::map<std::string, int> prior_red;

typedef std::vector<Light> Lights;

class Armor{
private:

public:
    cv::Rect2d armor_rect;
    Lights lights;
    int id;
    uint8_t box_color;

    explicit Armor(const cv::Rect& pos = cv::Rect2d(), const std::vector<Light>& blobs = std::vector<Light>(), 
                    uint8_t color = 0, int id = -1);

    cv::Point2f getCenter() const; // 获取装甲板中心
    double getBlobsDistance() const; // 获取两个灯条中心间距
    double lengthDistanceRatio() const; // 获取灯条中心距和灯条长度的比值
    // double getBoxDistance() const; // 获取装甲板到摄像头的距离

    double get_real_heigt() const; // 根据id获取真实高度 单位mm
    double get_real_width() const; // 根据id获取真实宽度 单位mm

    bool operator<(const Armor &box) const; // 装甲板优先级比较
};

#endif