#ifndef CAMERA_WRAPPER_H
#define CAMERA_WRAPPER_H

#include "Wrapper.hpp"
#include "CameraApi.h" // Some useful api from mind vision.
#include "../../../Tools/Include/RoundQueue.hpp"


class CameraWrapper: public Wrapper {
    friend void cameraCallback(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead,PVOID pContext);
private:
    const std::string name;
    int mode;

    bool init_done;

    unsigned char* rgb_buffer;
    int camera_cnts;
    int camera_status;
    tSdkCameraDevInfo camera_enum_list[2];
    int h_camera;
    char camera_name[32];

    tSdkCameraCapbility tCapability;
    tSdkFrameHead frame_info;
    BYTE *pby_buffer;
    IplImage* iplImage;
    int channel;

    RoundQueue<cv::Mat, 2> src_queue;
public:
    int gain;
    int exposure;

    CameraWrapper(int exposure, int gain, int camera_mode=1, const std::string &n="NULL");
    ~CameraWrapper() final;

    bool init() final;
    bool read(cv::Mat& src) final;
    bool readRaw(cv::Mat& src);
    bool readProcessed(cv::Mat& src);
    bool readCallback(cv::Mat& src);
};

#endif