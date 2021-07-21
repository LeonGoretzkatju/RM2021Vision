#include "Other/Serial/Include/SerialManager.h"
#include "Other/Camera/Include/CameraWrapper.h"
#include "Other/Camera/Include/VideoWrapper.h"
#include "Armor/Include/ArmorFinder.h"
#include "Predictor/Include/Predictor.h"
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

// void uart_receive(Serial* p_serial);

SerialManager* serial_manager = nullptr;
Wrapper* wrapper = nullptr;
Predictor* predictor = nullptr;

int main(){
    // serial_manager = new SerialManager();
    // Serial* serial = serial_manager->m_serial;
    // thread receive(uart_receive, serial);

    // uint8_t enemy_color = serial_manager->receive_data.enemy_color;
    predictor = new Predictor();
    ArmorFinder* armor_finder = new ArmorFinder(ENEMY_RED, serial_manager, predictor);
    cv::Mat src;
    // if from camera
    wrapper = new CameraWrapper(5, 100, 2);
    // wrapper = new VideoWrapper("../../test001.avi");
    wrapper->init();
    while (true) {
        
        // else if from video
        // wrapper = new VideoWrapper();

        bool change_mode = true;
        // 模式切换需要更新相机分辨率，等配置信息

        const uint8_t armor_mode = 0;
        const uint8_t energy_mode = 1;

        do{
            uint8_t state = armor_mode;
            // uint8_t state = serial_manager->receive_data.state;
            cout << "ckpt1: start reading image" << endl;
            wrapper->read(src);
            cout << "ckpt2: finish reading image" << endl;
            imshow("show figure", src);
            waitKey(1);
            cout << "ckpt3: finish showing image" << endl;
            switch(state){
                case armor_mode :
                    cout << "ckpt4: start running on this image." << endl;
                    armor_finder->run(src);
                    cout << "ckpt5: finish running on this image" << endl;
                    cout << endl;
                break;
                case energy_mode : 
                    // energy->run(src);
                break;
            }
        }while(change_mode);
        
    }

    // delete serial_manager;
    delete wrapper;

    return 0;

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