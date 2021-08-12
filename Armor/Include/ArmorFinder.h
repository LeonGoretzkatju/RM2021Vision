#ifndef __ARMOR_FINDER_H_
#define __ARMOR_FINDER_H_

#include "Armor.h"
#include "Classifier.h"
#include "../../Tools/Include/systime.h"
#include "../../Tools/Include/constants.h"
#include "../../Other/Serial/Include/SerialManager.h"
#include "Predictor.h"
#include "../../Tools/Include/Utils.h" 
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>
#include "../../Tools/Include/Draw_Curve.h"
#include "../../KCFcpp/src/kcftracker.hpp"
#include "../../mainwindow.h"


typedef std::vector<Armor> Armors;

class ArmorFinder{
private:
    Armors armors;
    // typedef cv::TrackerMIL TrackerToUse;                // Tracker类型定义
    typedef enum{
        SEARCHING_STATE, TRACKING_STATE
    } State;                                            // 自瞄状态枚举定义

    systime frame_time;                                 // 当前帧对应时间
    const uint8_t &enemy_color;                         // 敌方颜色，引用外部变量，自动变化
    State state;                                        // 自瞄状态对象实例
    Armor target_box, last_box;                         // 目标装甲板
    int anti_switch_cnt;                                // 防止乱切目标计数器
    // cv::Ptr<cv::Tracker> tracker;                       // tracker对象实例
    int contour_area; 
    KCFTracker *tracker;
    Classifier classifier;                              // CNN分类器对象实例，用于数字识别
    MainWindow *w_;
    int tracking_cnt;                                   // 记录追踪帧数，用于定时退出追踪

    SerialManager* serial_manager; // 与串口交互
    Predictor* predictor; // 与预测器交互

    bool findLights(const cv::Mat& src, Lights& lights);
    bool findArmors(const cv::Mat& src, Armor& box);
    bool matchArmors(const cv::Mat& src, const Lights& lights, Armors& armors);

    bool stateSearchingTarget(const cv::Mat& src);            // searching state主函数
    bool stateTrackingTarget(cv::Mat& src);             // tracking state主函数
public:
    ArmorFinder(const uint8_t& color, SerialManager* serial_manager, Predictor* predictor, const string &paras_folder);
    ~ArmorFinder() = default;
    DrawCurve* drawCurve;
    void run(cv::Mat& src);
    void DebugPlotInit(MainWindow *w);

};

#endif