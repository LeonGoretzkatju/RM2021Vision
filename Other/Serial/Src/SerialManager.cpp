#include "SerialManager.h"
#include "CRC_Check.h"
#include <iostream>

extern SerialManager* serial_manager;


SerialManager::SerialManager(){
    m_serial = new Serial();
}

SerialManager::~SerialManager(){
    delete m_serial;
}


void uart_receive(Serial *p_serial){
    // cout << "thread" << endl;
    char buffer[40];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        // cout << "size of the receive data " << "                 "<< sizeof(ReceiveData) << endl;
        p_serial->read_data((uint8_t *) buffer, sizeof(ReceiveData)+1);
        if (buffer[sizeof(ReceiveData)] == '\n')
        memcpy(&serial_manager->receive_data, buffer, sizeof(ReceiveData));
    }
}

// 传参
void SerialManager::uart_send(Point2f angle, Point2f origin_angle, bool fire, bool ArmorFound){
    char buffer[40];
    send_data.final_pitch = angle.y;
    send_data.final_yaw = angle.x;
    send_data.origin_yaw = origin_angle.x;
    send_data.origin_pitch = origin_angle.y;
    double now_yaw = serial_manager->receive_data.curr_yaw;
    double now_pitch = serial_manager->receive_data.curr_pitch;

    // if(abs(now_pitch - angle.y) < 0.001){
    //     send_data.final_pitch = now_pitch;
    // }
    // if(abs(now_yaw - angle.x) < 0.008){
    //     send_data.final_yaw = now_yaw;
    // }

    // std::cout << "send yaw: " << send_data.final_yaw << std::endl;
    // std::cout << "send pitch: " << send_data.final_pitch << std::endl;

    send_data.fire = fire;
    send_data.ArmorFound = ArmorFound;
    memset(buffer, 0, sizeof(buffer));
    memcpy(&buffer, (void *)&send_data, sizeof(SendData));
    Serial* serial = this->m_serial;
    if(serial->write_data((uint8_t*) buffer, sizeof(send_data))){
        // cout << "send" << endl;
    }
}

void SerialManager::TransformData(const VisionData &data)
{

    Tdata[0] = 0xA5;

    Tdata[1] = CmdID1;
    Append_CRC8_Check_Sum(Tdata, 3);

    Tdata[3] = data.pitch_angle.c[0];
    Tdata[4] = data.pitch_angle.c[1];
    Tdata[5] = data.pitch_angle.c[2];
    Tdata[6] = data.pitch_angle.c[3];

    Tdata[7] = data.yaw_angle.c[0];
    Tdata[8] = data.yaw_angle.c[1];
    Tdata[9] = data.yaw_angle.c[2];
    Tdata[10] = data.yaw_angle.c[3];

    Tdata[11] = data.dis.c[0];
    Tdata[12] = data.dis.c[1];
    Tdata[13] = data.dis.c[2];
    Tdata[14] = data.dis.c[3];

    Tdata[15] = data.ismiddle;
    Tdata[16] = data.isFindTarget;

    Tdata[17] = data.isfindDafu;
    Tdata[18] = 0x00;
    Tdata[19] = data.nearFace;

    Append_CRC16_Check_Sum(Tdata, 22);
    m_serial->send_data(Tdata);

}
