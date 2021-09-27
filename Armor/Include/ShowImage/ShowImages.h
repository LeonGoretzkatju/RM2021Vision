#ifndef _SHOW_IMAGES_H_
#define _SHOW_IMAGES_H_

#include <opencv2/core.hpp>
#include "../ArmorFinder.h"
#include "../Armor.h"

//
void showArmorBoxes(std::string windows_name, const cv::Mat &src, const Armors &armor_boxes);
void showArmorBox(std::string windows_name, const cv::Mat &src, const Armor &armor_box);
void showLightBlobs(std::string windows_name, const cv::Mat &src, const Lights &light_blobs);
void showTrackSearchingPos(std::string window_names, const cv::Mat &src, const cv::Rect2d pos);

#endif