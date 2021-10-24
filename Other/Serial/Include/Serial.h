#ifndef __serial_h_
#define __serial_h_

#include <string>

using namespace std;

class Serial{
private:
    int fd; // 设备文件操作符
    int n_speed; // 波特率
    char n_event; // 奇偶校验
    int n_bits; // 位长
    int n_stop; // 停止符
    bool wait_uart = false;

    bool set_opt(int fd, int n_speed, char n_event, int n_bits, int n_stop);

public:
    Serial(int n_speed = 115200, char n_event = 'N', int n_bits = 8, int n_stop = 1);
    ~Serial();

    string get_uart_dev_name();
    bool init_port(int n_speed = 115200, char n_event = 'N', int n_bits = 8, int n_stop = 1);
    bool write_data(const unsigned char* p_data, unsigned int length);
    bool read_data(unsigned char* buffer, unsigned int length);
    void send_data(unsigned char Tdata[30]);

};


#endif