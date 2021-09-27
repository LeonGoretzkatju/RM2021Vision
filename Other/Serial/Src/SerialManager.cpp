#include "SerialManager.h"
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
