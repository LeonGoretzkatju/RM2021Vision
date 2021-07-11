#include "Other/Include/SerialManager.h"
#include <thread>

using namespace std;

// void uart_receive(Serial* p_serial);

SerialManager* serial_manager = nullptr;

int main(){
    serial_manager = new SerialManager();
    Serial* serial = serial_manager->get_serial();
    thread receive(uart_receive, serial);
    
}