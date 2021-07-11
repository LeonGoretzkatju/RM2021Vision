#include "Other/Include/SerialManager.h"
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

// void uart_receive(Serial* p_serial);

SerialManager* serial_manager = nullptr;

int main(){
    serial_manager = new SerialManager();
    Serial* serial = serial_manager->m_serial;
    thread receive(uart_receive, serial);
    float i = 0;
    while(true){
        Point2f test;
        test.x = 3.14;
        test.y = 2.71;
        cout << serial_manager->receive_data.curr_pitch << endl
             << serial_manager->receive_data.curr_yaw << endl
             << serial_manager->receive_data.shoot_speed << endl;
        serial_manager->uart_send(test, true);
        sleep(0.1);
    }
}