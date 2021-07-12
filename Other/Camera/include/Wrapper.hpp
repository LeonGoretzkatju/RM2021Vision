#ifndef WRAPPER__H
#define WRAPPER__H

#include <opencv2/core/core.hpp>


class Wrapper {
public:
    virtual ~Wrapper() = default;;
    virtual bool init() = 0;
    virtual bool read(cv::Mat &src) = 0;
};

#endif
