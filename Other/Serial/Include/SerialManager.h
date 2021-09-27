#ifndef __serial_manager_h_
#define __serial_manager_h_

#include <opencv2/core/core.hpp>
#include "Serial.h"

using namespace cv;
// 这里完成对串口消息的封装，应用层
// 文件传输结构

struct ReceiveData{
    float shoot_speed;   // 射速
    float curr_yaw;      // 当前云台yaw角度
    float curr_pitch;    // 当前云台pitch角
    uint8_t state;       // 当前状态，自瞄-大符-小符
    // // uint8_t mark;        // 云台角度标记位
    uint8_t enemy_color; // 敌方颜色
};

struct SendData{
    float final_yaw;    // 最终偏转角
    float final_pitch;
    float origin_yaw;
    float origin_pitch;
    bool fire;          // 发射指令
    bool ArmorFound;
};

void uart_receive(Serial *p_serial);

class SerialManager{
private:

public:
    SerialManager();
    ~SerialManager();

    ReceiveData receive_data;
    SendData send_data;

    void uart_send(Point2f angle, Point2f origin_angle, bool fire, bool ArmorFound);
    Serial* m_serial;
};


#endif