#include "../../Include/ShowImage/ShowImages.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include "../../../Tools/Include/Options.h"

using namespace cv;


void drawLightBlobs(cv::Mat &src, const Lights &blobs){
    for (const auto &blob:blobs) {
        Scalar color(0,255,0);
        if (blob.color == BLOB_RED)
            color = Scalar(0, 0, 255);
        else if (blob.color == BLOB_BLUE)
            color = Scalar(255, 0, 0);
        cv::Point2f vertices[4];
        blob.light_rect.points(vertices);
        for (int j = 0; j < 4; j++) {
            cv::line(src, vertices[j], vertices[(j + 1) % 4], color, 2);
        }
    }
}

/**************************
 *     显示多个装甲板区域    *
 **************************/
void showArmorBoxes(std::string windows_name, const cv::Mat &src, const Armors &armor_boxes) {
    static Mat image2show;
    if (src.type() == CV_8UC1) {// 黑白图像
        cvtColor(src, image2show, COLOR_GRAY2RGB);
    } else if (src.type() == CV_8UC3) { //RGB 彩色
        image2show = src.clone();
    }

    for (auto &box:armor_boxes) {
        if(box.box_color == BOX_BLUE) {
            rectangle(image2show, box.armor_rect, Scalar(0, 255, 0), 1);
            drawLightBlobs(image2show, box.lights);
        }else if(box.box_color == BOX_RED){
            rectangle(image2show, box.armor_rect, Scalar(0, 255, 0), 1);
            drawLightBlobs(image2show, box.lights);
        }

    }
    imshow(windows_name, image2show);
}

/**************************
 * 显示多个装甲板区域及其类别 *
 **************************/
void showArmorBoxesClass(std::string window_names, const cv::Mat &src, const Armors &boxes) {
    static Mat image2show;
    if (src.type() == CV_8UC1) { // 黑白图像
        cvtColor(src, image2show, COLOR_GRAY2RGB);
    } else if (src.type() == CV_8UC3) { //RGB 彩色
        image2show = src.clone();
    }

    for (const auto &box : boxes) {
        if(box.id != 0) {
            rectangle(image2show, box.armor_rect, Scalar(0, 255, 0), 1);
            drawLightBlobs(image2show, box.lights);
            if (box.id == -1)
                putText(image2show, std::to_string(box.id), Point(box.armor_rect.x + 2, box.armor_rect.y + 2), cv::FONT_HERSHEY_TRIPLEX, 1,
                        Scalar(0, 255, 0));
            else if (1 <= box.id && box.id < 8)
                putText(image2show, std::to_string(box.id), Point(box.armor_rect.x + 2, box.armor_rect.y + 2), cv::FONT_HERSHEY_TRIPLEX, 1,
                        Scalar(255, 0, 0));
            else if (8 <= box.id && box.id < 15)
                putText(image2show, std::to_string(box.id), Point(box.armor_rect.x + 2, box.armor_rect.y + 2), cv::FONT_HERSHEY_TRIPLEX, 1,
                        Scalar(0, 0, 255));
            else if (box.id != 0)
                std::cout << "invaild armor type" << endl;
        }
    }
    imshow(window_names, image2show);

}


/**************************
 *      显示多个灯条区域     *
 **************************/
void showLightBlobs(std::string windows_name, const cv::Mat &src, const Lights &light_blobs) {
    static Mat image2show;

    if (src.type() == CV_8UC1) { // 黑白图像
        cvtColor(src, image2show, COLOR_GRAY2RGB);
    } else if (src.type() == CV_8UC3) { //RGB 彩色
        image2show = src.clone();
    }

    for (const auto &light_blob:light_blobs) {
        Scalar color(0, 255, 0);
        if (light_blob.color == BLOB_RED)
            color = Scalar(0, 0, 255);
        else if (light_blob.color == BLOB_BLUE)
            color = Scalar(255, 0, 0);
        cv::Point2f vertices[4];
        light_blob.light_rect.points(vertices);
        for (int j = 0; j < 4; j++) {
            cv::line(image2show, vertices[j], vertices[(j + 1) % 4], color, 2);
        }
    }
    imshow(windows_name, image2show);
}


void showTrackSearchingPos(std::string window_names, const cv::Mat &src, const cv::Rect2d pos){
    static Mat image2show;
    if (src.type() == CV_8UC1) { // 黑白图像
        cvtColor(src, image2show, COLOR_GRAY2RGB);
    } else if (src.type() == CV_8UC3) { //RGB 彩色
        image2show = src.clone();
    }
    rectangle(image2show, pos, Scalar(0, 255, 0), 1);
    imshow(window_names, image2show);
}

void showArmorBox(std::string windows_name, const cv::Mat &src, const Armor &box) {
    static Mat image2show;
    if(box.armor_rect == cv::Rect2d()){
        imshow(windows_name, src);
    }
    if (src.type() == CV_8UC1) { // 黑白图像
        cvtColor(src, image2show, COLOR_GRAY2RGB);
    } else if (src.type() == CV_8UC3) { //RGB 彩色
        image2show = src.clone();
    }
    drawLightBlobs(image2show, box.lights);
//    static FILE *fp = fopen(PROJECT_DIR"/ratio.txt", "w");
//    if(box.light_blobs.size() == 2)
//        fprintf(fp, "%lf %lf %lf\n", box.light_blobs[0].length, box.light_blobs[1].length, box.getBlobsDistance())
//    cout << box.lengthDistanceRatio() << endl;

    rectangle(image2show, box.armor_rect, Scalar(0, 255, 0), 3);

    if (box.id == -1)
        putText(image2show, id2name[box.id], Point(box.armor_rect.x + 2, box.armor_rect.y + 2), cv::FONT_HERSHEY_TRIPLEX, 1,
                Scalar(0, 255, 0));
    else if (1 <= box.id && box.id < 8)
        putText(image2show, id2name[box.id], Point(box.armor_rect.x + 2, box.armor_rect.y + 2), cv::FONT_HERSHEY_TRIPLEX, 1,
                Scalar(255, 0, 0));
    else if (8 <= box.id && box.id < 15)
        putText(image2show, id2name[box.id], Point(box.armor_rect.x + 2, box.armor_rect.y + 2), cv::FONT_HERSHEY_TRIPLEX, 1,
                Scalar(0, 0, 255));
    imshow(windows_name, image2show);
}