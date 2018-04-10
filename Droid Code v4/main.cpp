#include <iostream>
#include <string.h>
#include <zconf.h>
#include <EngineController.h>
#include <ArduinoController.h>

using namespace std;


int main() {
    ArduinoController ctrl("/dev/ttyACM1");
    ctrl.step(20, 100);
    return 0;
}