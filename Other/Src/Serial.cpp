#include <unistd.h>     /*Unix标准函数定义*/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>   /*PPSIX终端控制定义*/
#include <iostream>
#include <string.h>

#include "Serial.h"

using namespace std;

// default config
Serial::Serial(int n_speed = 115200, char n_event = 'N', int n_bits = 8, int n_stop = 1) : 
            n_speed(n_speed), n_event(n_event), n_bits(n_bits), n_stop(n_stop){
    if(!InitPort(n_speed, n_event, n_bits, n_stop)){
        cout << "cannot init serial" << endl;
    }
}

Serial::~Serial(){
    if(!close(fd)){
        cout << "" << endl;
    }
}

bool Serial::InitPort(int n_speed = 115200, char n_event = 'N', int n_bits = 8, int n_stop = 1){
    const char* ttl = "/dev/ttyTHS0";
    if((fd = open(ttl, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){
        return false;
    }

    if(!set_opt(fd, n_speed, n_event, n_bits, n_stop)){
        return false;
    }
}

bool Serial::set_opt(int fd, int n_speed, char n_event, int n_bits, int n_stop){
    
    if(fcntl(fd, F_SETFL, 0) < 0){
        cout << "fcntl failed" << endl;
        return false;
    }

    termios newtio{}, oldtio{};
    if (tcgetattr(fd, &oldtio) != 0) {
        perror("SetupSerial 1");
        return -1;
    }
    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    // 设置字符大小
    switch (n_bits) {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
        default:
            break;
    }

    switch (n_event) {
        case 'O':  //奇校验
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':  //偶校验
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':  //无校验
            newtio.c_cflag &= ~PARENB;
            break;
        default:
            break;
    }

    // 设置波特率
    switch (n_speed) {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }

    // 设置停止位
    if (n_stop == 1) {
        newtio.c_cflag &= ~CSTOPB;
    } else if (n_stop == 2) {
        newtio.c_cflag |= CSTOPB;
    }

    // 设置等待时间，最小字符
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);
    // 重新激活
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
        perror("com set error");
        return false;
    }

    printf("set done!\n");

    return true;
}

bool Serial::WriteData(const unsigned char* p_data, unsigned int length){
    if(write(fd, p_data, length) == -1) {
        cout << "write failed" << endl;
        return false;
    }
    return true;
}

bool Serial::ReadData(unsigned char* buffer, unsigned int length){
    if(read(fd, buffer, length) == -1){
        cout << "read failed" << endl;
        return false;
    }
    return true;
}