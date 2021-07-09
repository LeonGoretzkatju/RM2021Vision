#include "SerialManager.h"

extern  SerialManager serial_manager;

void uart_receive(Serial *p_serial){
    char buffer[40];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        p_serial->read_data((uint8_t *) buffer, sizeof(ReceiveData));
        memcpy(&serial_manager.receive_data, buffer, sizeof(ReceiveData));
    }
}

// 传参
void SerialManager::uart_send(Point2f angle, bool fire){
    char buffer[40];
    send_data.final_pitch = angle.y;
    send_data.final_yaw = angle.x;
    send_data.fire = fire;
    memset(buffer, 0, sizeof(buffer));
    memcpy(&buffer, (void *)&send_data, sizeof(send_data));
    this->m_serial.write_data((uint8_t*) buffer, sizeof(send_data));
    
}
