#ifndef __serial_manager_h_
#define __serial_manager_h_

#include "Serial.h"
#include <opencv2/core/core.hpp>

using namespace cv;
// 这里完成对串口消息的封装，应用层
// 文件传输结构
struct ReceiveData{
    int shoot_speed;

};

struct SendData{

};

void uartReceive(Serial *p_serial);

class SerialManager{
private:
    Serial serial;
    enum state{send, receive};
public:
    Point2f sendAngle();

    Serial getSerial(){
        return serial;
    }
};


#endif