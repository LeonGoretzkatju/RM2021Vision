#ifndef __serial_manager_h_
#define __serial_manager_h_

#include <opencv2/core/core.hpp>
#include "Serial.h"

using namespace cv;
// 这里完成对串口消息的封装，应用层
// 文件传输结构

#define TRUE 1
#define FALSE 0

//模式
#define CmdID0 0x00; //关闭视觉
#define CmdID1 0x01; //识别红色
#define CmdID2 0x02; //识别蓝色
#define CmdID3 0x03; //小幅
#define CmdID4 0x04; //大幅

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

//字节数为4的结构体
typedef union
{
    float f;
    unsigned char c[4];
} float2uchar;

//字节数为2的uchar数据类型
typedef union
{
    int16_t d;
    unsigned char c[2];
} int16uchar;

//用于保存目标相关角度和距离信息及瞄准情况
typedef struct
{
    float2uchar yaw_angle;//偏航角
    float2uchar pitch_angle;//俯仰角
    float2uchar dis;//目标距离
    int ismiddle;//设置1表示目标进入了可以开火的范围，设置0则表示目标尚未进入可开火的范围，目前暂不使用，默认置0
    int isFindTarget;//当识别的图片范围内有目标且电控发来的信号不为0x00（关闭视觉）置为1，否则置0
    int isfindDafu;
    int nearFace;
} VisionData;

void uart_receive(Serial *p_serial);

class SerialManager{
private:

public:
    unsigned char Tdata[30];  //transfrom data
    SerialManager();
    ~SerialManager();

    ReceiveData receive_data;
    SendData send_data;

    void uart_send(Point2f angle, Point2f origin_angle, bool fire, bool ArmorFound);
    void TransformData(const VisionData &data);
    Serial* m_serial;
};


#endif