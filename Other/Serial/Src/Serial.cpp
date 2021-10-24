#include <unistd.h>     /*Unix标准函数定义*/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>   /*PPSIX终端控制定义*/
#include <iostream>
#include <string.h>

#include "Serial.h"

using namespace std;

// default config
Serial::Serial(int n_speed, char n_event, int n_bits, int n_stop) : 
            n_speed(n_speed), n_event(n_event), n_bits(n_bits), n_stop(n_stop){
    if (wait_uart) {
        printf("Wait for serial be ready!");
        while (!init_port(n_speed, n_event, n_bits, n_stop));
        printf("Port set successfully!");
    } else {
        if (init_port(n_speed, n_event, n_bits, n_stop)) {
            printf("Port set successfully!");
        } else {
            printf("Port set fail!");
        }
    }
}

Serial::~Serial(){
    if(!close(fd)){
        cout << "error close" << endl;
    }
}

string Serial::get_uart_dev_name() {
    FILE *ls = popen("ls /dev/ttyUSB* --color=never", "r");
    char name[20] = {0};
    fscanf(ls, "%s", name);
    return name;
}

bool Serial::init_port(int n_speed, char n_event, int n_bits, int n_stop){
    string name = get_uart_dev_name();
    cout << name << endl;
    if (name == "") {
        return false;
    }

    int chmod = system("echo tjurm | sudo -S chmod 777 /dev/ttyUSB*");
    if(chmod == -1){
        cout << "chmod false" << endl;
    }

    if ((fd = open(name.data(), O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        cout << "maybe chmod" <<endl;
        return false;
    }

    if(!set_opt(fd, n_speed, n_event, n_bits, n_stop)){
        return false;
    }
    return true;
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

bool Serial::write_data(const unsigned char* p_data, unsigned int length){
    int cnt = 0, curr = 0;
    if (fd <= 0){
        if(wait_uart){
            init_port(n_speed, n_event, n_bits, n_stop);
        }
        return false;
    }
    while ((curr = write(fd, p_data + cnt, length - cnt)) > 0 && (cnt += curr) < length);
    if (curr < 0) {
        printf("Serial offline write!");
        close(fd);
        if (wait_uart) {
            init_port(n_speed, n_event, n_bits, n_stop);
        }
        return false;
    }
    return true;
}

bool Serial::read_data(unsigned char* buffer, unsigned int length){
    int cnt = 0, curr = 0;
    while ((curr = read(fd, buffer + cnt, length - cnt)) > 0 && (cnt += curr) < length);
    if (curr < 0) {
        printf("Serial offline read!");
        close(fd);
        if (wait_uart) {
            init_port(n_speed, n_event, n_bits, n_stop);
        }
        return false;
    }
    return true;
}

void Serial::send_data(unsigned char *Tdata) {
    write(fd,Tdata,22);
}