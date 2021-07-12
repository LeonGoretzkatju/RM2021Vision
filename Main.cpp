#include "Other/Serial/Include/SerialManager.h"
#include "Other/Camera/Include/CameraWrapper.hpp"
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

// void uart_receive(Serial* p_serial);

SerialManager* serial_manager = nullptr;
CameraWrapper* camera_wrapper = nullptr;

int main(){
    serial_manager = new SerialManager();
    camera_wrapper = new CameraWrapper(5, 100, 2);
    //
    camera_wrapper->init();

    cv::Mat src;
    while (true) {
        camera_wrapper->readCallback(src);
        cv::imshow("test camera wrapper.", src);
        cv::waitKey(1);
    }

    // Serial* serial = serial_manager->m_serial;
    // thread receive(uart_receive, serial);
    // float i = 0;
    // Point2f test;
    // test.x = 3.14;
    // test.y = 2.71;
    // while(true){
    //     test.x += 0.01;
    //     test.y += 0.01;
    //     cout << serial_manager->receive_data.curr_pitch << endl
    //          << serial_manager->receive_data.curr_yaw << endl
    //          << serial_manager->receive_data.shoot_speed << endl;
    //     serial_manager->uart_send(test, true);
    //     cout << "test.x" << test.x << endl;
    //     cout << "test.y" << test.y << endl;
    //     usleep(1000);
    // }
}