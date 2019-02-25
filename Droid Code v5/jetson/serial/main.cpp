// main.cpp
// Griffith Droid Racer 2019
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

#define ARDUINO_PORT "/dev/ttyS3"

#include "ArduinoController.h"

int main()
{
    ArduinoController arduino(ARDUINO_PORT);
    
    // Start communications
    std::string response = arduino.receive();
    printf("%s\n", response.c_str());

    for (int commandIndex = 0; true; commandIndex++)
    {
        printf("Normalised power: %d\n", arduino.getPower());
        printf("Normalised angle: %d\n", arduino.getAngle());

        printf("> ");
        std::string input;
        std::cin >> input;
        
        if (input.compare("q") == 0)
        {
            printf("Goodbye!\n");
            break;
        }
        else if(input.compare("p") == 0)
        {
            printf("power = ");
            std::cin >> input;
            arduino.setPower(std::stoi(input));
        }
        else if(input.compare("a") == 0)
        {
            printf("angle = ");
            std::cin >> input;
            
            int angle = std::stoi(input);
            
            arduino.setAngle(angle);
        }
    }

    return 0;
}