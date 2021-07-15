#ifndef _VIDEO_WRAPPER_H_
#define _VIDEO_WRAPPER_H_


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Wrapper.h"


class VideoWrapper:public Wrapper {
private:
    cv::VideoCapture video;

public:
    VideoWrapper(const std::string& filename);
    ~VideoWrapper();

    bool init() final;
    bool read(cv::Mat &src) final;
};

#endif
