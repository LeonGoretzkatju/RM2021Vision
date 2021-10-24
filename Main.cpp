#include "Other/Serial/Include/SerialManager.h"
#include "Other/Camera/Include/CameraWrapper.h"
#include "Other/Camera/Include/VideoWrapper.h"
#include "Armor/Include/ArmorFinder.h"
#include "Predictor/Include/Predictor.h"
#include <iostream>
#include <thread>
#include <QTime>
#include <unistd.h>
#include "log.hpp"

using namespace std;

// void uart_receive(Serial* p_serial);

// 只需要被一个类使用，采用包含
// 需要被其他功能模块复用，采用委托
SerialManager* serial_manager = nullptr;
Wrapper* wrapper = nullptr;
Predictor* predictor = nullptr;

int main(){
    serial_manager = new SerialManager();
    Serial* serial = serial_manager->m_serial;
    thread receive(uart_receive, serial);

    // uint8_t enemy_color = serial_manager->receive_data.enemy_color;
    predictor = new Predictor();
    ArmorFinder* armor_finder = new ArmorFinder(ENEMY_RED, serial_manager, predictor, "/home/nuc/rm2021/Tools/para/");
    cv::Mat src;
    // if from camera
    //wrapper = new CameraWrapper(10, 100, 2);
    wrapper = new VideoWrapper("/home/nuc/hero.avi");
    wrapper->init();
    while (true) {
        
        // else if from video
        // wrapper = new VideoWrapper();

        bool change_mode = true;
        // 模式切换需要更新相机分辨率，等配置信息

        const uint8_t armor_mode = 1;
        const uint8_t energy_mode = 0;
        // int argc;char **argv = nullptr;
        // QApplication a(argc, argv);
        // MainWindow w;
        // w.show();

        do{
            cout << serial_manager->receive_data.curr_yaw << endl
                      << serial_manager->receive_data.curr_pitch << endl
                      << serial_manager->receive_data.shoot_speed << endl
                      << (char)serial_manager->receive_data.state << endl
                      << (char)serial_manager->receive_data.enemy_color << endl;
            uint8_t state = armor_mode;
            // uint8_t state = serial_manager->receive_data.state;
            wrapper->read(src);
            double t = getTickCount();
            switch(state){
                case armor_mode :
                    // armor_finder->DebugPlotInit(&w);
                    armor_finder->run(src);
                break;
                case energy_mode : 
                    // energy->run(src);
                break;
            }
            double t1=(getTickCount()-t)/getTickFrequency();
            printf("Armor Detecting FPS: %f\n",1/t1);

        }while(change_mode);
        
    }

    // delete serial_manager;
    delete wrapper;

    return 0;
}