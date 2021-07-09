#include "SerialManager.h"
#include <thread>

using namespace std;

extern void uartReceive(Serial* p_serial);

SerialManager* serial_manager = new SerialManager();

int main(){
    thread receive(uartReceive, &serial_manager->getSerial());
}