// ArduinoController.cpp
// Griffith Droid Racer 2019

#include "ArduinoController.h"

ArduinoController::ArduinoController(std::string portName)
{
    this->serialPort = open(portName.c_str(), O_RDWR);

    if (this->serialPort < 0)
    {
        printf("Error %i from opening port: %s\n", errno, strerror(errno));
    }

    this->configureTty();

    this->setAngle(50);
    this->setPower(0);
}

ArduinoController::~ArduinoController()
{
    close(this->serialPort);
}

int ArduinoController::configureTty()
{
    memset(&this->tty, 0, sizeof(this->tty));

    if (tcgetattr(this->serialPort, &this->tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return errno;
    }

    this->tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
    this->tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    this->tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    this->tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    this->tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    this->tty.c_lflag &= ~ICANON;
    this->tty.c_lflag &= ~ECHO;                                                        // Disable echo
    this->tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    this->tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    this->tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    this->tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
    this->tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    this->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    this->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    this->tty.c_cc[VTIME] = 1;  // Wait for up to 1 deciseconds, returning as soon as any data is received
    this->tty.c_cc[VMIN] = 255; // Accept up to 255 bytes

    cfsetispeed(&(this->tty), this->BAUD_RATE); // Set baud-in to 9600
    cfsetospeed(&(this->tty), this->BAUD_RATE); // Set baud-out to 9600

    if (tcsetattr(this->serialPort, TCSANOW, &(this->tty)) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return errno;
    }

    return 0;
}

std::string ArduinoController::receive()
{
    std::string buffer;
    int r = read(this->serialPort, &buffer, sizeof(buffer));

    if (r < 0)
    {
        printf("Error reading: %s", strerror(errno));
    }

    return buffer;
}

int ArduinoController::update()
{
    char command[16];
    int r;

    sprintf(command, "%d,%d", this->angle, this->power);

    r = write(this->serialPort, command, strlen(command));
    return r;
}

int ArduinoController::getNormalisedPower(int rawPower)
{
    if (rawPower > 100)
    {
        rawPower = 100;
    }
    else if (rawPower <= 0)
    {
        return this->NO_POWER;
    }
    int normalisedPower = ((this->MAX_POWER - this->MIN_POWER) / 100) * rawPower + MIN_POWER;
    return normalisedPower;
}

int ArduinoController::getNormalisedAngle(int rawAngle)
{
    if (rawAngle > 100)
    {
        rawAngle = 100;
    }
    else if (rawAngle <= 0)
    {
        return this->MIN_ANGLE;
    }
    int normalisedAngle = ((this->MAX_ANGLE - this->MIN_ANGLE) / 100) * rawAngle + MIN_ANGLE;

    return normalisedAngle;
}

int ArduinoController::getRawPower(int normalisedPower)
{
    int rawPower = (normalisedPower - MIN_POWER) / ((this->MAX_POWER - this->MIN_POWER) / 100.0);
    return rawPower;
}

int ArduinoController::getRawAngle(int normalisedAngle)
{
    int rawAngle = (normalisedAngle / MIN_ANGLE) / ((this->MAX_ANGLE - this->MIN_ANGLE) / 100.0);
    return rawAngle;
}

void ArduinoController::setPower(int power)
{
    // power should be in the range of 0-100.
    this->power = this->getRawPower(power);
    this->update();
}

void ArduinoController::setAngle(int angle)
{
    this->angle = this->getRawAngle(angle);
    this->update();
}

int ArduinoController::getPower()
{
    return this->getNormalisedPower(this->power);
}

int ArduinoController::getAngle()
{
    return this->getNormalisedAngle(this->angle);
}

void ArduinoController::stop()
{
    this->setPower(0);
    this->setAngle(0);
}