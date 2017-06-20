#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "ArduinoStream.h"

int main()
{
    ArduinoStream stream("/dev/ttyACM0", "9600");
    stream.Init();

    stream.Send("motor 1010");
    stream.Send("turn angle");
    cout << stream.GetPreviousCommand();

    return 0;
}
