#include "../Include/ArmorFinder.h"
#include "../../Tools/Include/systime.h"
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>

using namespace std;

ArmorFinder::ArmorFinder(const uint8_t &color) :
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
            // cout << "searching state" << endl;
            if (stateSearchingTarget(src)) {
                if ((target_box.armor_rect & cv::Rect2d(0, 0, 640, 480)) == target_box.armor_rect) { // 判断装甲板区域是否脱离图像区域
                    // tracker = TrackerToUse::create();                       // 成功搜寻到装甲板，创建tracker对象
                    // tracker->init(src, target_box.armor_rect);
                    
                    // TODO: 添加当前串口信息
                    state = TRACKING_STATE;
                    tracking_cnt = 0;
                    // cout << "into track" << endl;
                }
            }
            break;
        case TRACKING_STATE:
            // cout << "tracking state" << endl;
            if (!stateTrackingTarget(src) || ++tracking_cnt > 100) {    // 最多追踪100帧图像
                state = SEARCHING_STATE;
                // cout << "into searching" << endl;
            } else {
                // 获取target box的四个点坐标
                Rect2d box = this->target_box.armor_rect;
                
                // 装甲板实际宽高
                float armor_real_height = this->target_box.get_real_heigt();
                float armor_real_width = this->target_box.get_real_width();

                // 特征点世界坐标（以特征点所在平面为xy）
                std::vector<Point3f> obj_pnts;
                // left top point.
                obj_pnts.emplace_back(Point3f(-0.5 * armor_real_width, -0.5 * armor_real_height, 0));
                // right top point.
                obj_pnts.emplace_back(Point3f(0.5 * armor_real_width, -0.5 * armor_real_height, 0));
                // right bottom point.
                obj_pnts.emplace_back(Point3f(0.5 * armor_real_width, 0.5 * armor_real_height, 0));
                // left bottom point.
                obj_pnts.emplace_back(Point3f(-0.5 * armor_real_width, 0.5 * armor_real_height, 0));

                // 特征点图像坐标
                std::vector<Point2f> img_pnts;
                img_pnts.emplace_back(Point2f(box.x, box.y));
                img_pnts.emplace_back(Point2f(box.x + box.width, box.y));
                img_pnts.emplace_back(Point2f(box.x + box.width, box.y + box.height));
                img_pnts.emplace_back(Point2f(box.x, box.y + box.height));

                // 相机内参矩阵 + 畸变矩阵
                // fx, 0, cx
                // 0, fy, cy
                // 0, 0, 1
                Mat inner_matrix = (Mat_<double>(3, 3) << 933.94931947048565, 0, 320.0,
                                                          0, 933.94931947048565, 240.0,
                                                          0, 0, 1);
                Mat distortion_matrix = (Mat_<double>(5, 1) << 0.0528381602513808, -1.9140458072801805, 0.0, 0.0, 9.1338656641243716);

                // solve pnp
                Mat rvecs = cv::Mat::zeros(3, 1, CV_64FC1);
                Mat tvecs = cv::Mat::zeros(3, 1, CV_64FC1);
                // cv::Mat Rod_r ,TransMatrix ,RotationR;
                bool success = cv::solvePnP(
                    obj_pnts, img_pnts, inner_matrix, distortion_matrix, rvecs, tvecs
                );
                // Rodrigues(Rod_r, RotationR);
                // cout << "C(Camera center:):" << endl << -RotationR.inv()*TransMatrix << endl;//这个C果然是相机中心，十分准确
                cout << tvecs.ptr<double>(0)[0] << " "
                     << tvecs.ptr<double>(0)[1] << " "
                     << tvecs.ptr<double>(0)[2] << " "
                     << endl;
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