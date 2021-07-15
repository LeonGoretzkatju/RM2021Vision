#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>


class Wrapper {
public:
    virtual ~Wrapper() = default;
    virtual bool init() = 0;
    virtual bool read(cv::Mat &src) = 0;
};

#endif
