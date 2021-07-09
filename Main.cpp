#include "SerialManager.h"
#include <thread>

using namespace std;

extern void uart_receive(Serial* p_serial);

SerialManager serial_manager;

int main(){
    thread receive(uart_receive, &serial_manager.get_serial());
}