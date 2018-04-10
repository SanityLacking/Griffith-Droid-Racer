#ifndef ARDUINOSTREAM_H_INCLUDED
#define ARDUINOSTREAM_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

class ArduinoStream
{
    bool initialised = false;
    string init;
    string bufferClear;
    string serialPort;
    string baudRate;
    string previousCommand;
    ofstream ardOut;
    ifstream ardIn;
public:
    //ArduinoStream();
    ArduinoStream(string, string);
    ~ArduinoStream();
    void Init();
    void Send(string);
    string GetPreviousCommand();
};

ArduinoStream::ArduinoStream(string sPort = "/dev/ttyACM0", string bRate = "9600") {
    /* initialise consts */
    this->serialPort = sPort;
    this->baudRate = bRate;

    /* initialise first stream openers */
    this->init = "sudo stty -F " + this->serialPort + " cs8 " + this->baudRate + " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts";
    this->bufferClear = "echo \"Hello, Arduino.\" | sudo tee --append " + this->serialPort;

    /* open io file streams to Arduino */
    this->ardOut.open(this->serialPort, ofstream::out | ofstream::app);
    this->ardIn.open (this->serialPort, ifstream::in);

    /* set flag */
    this->initialised = true;
}

ArduinoStream::~ArduinoStream() {
    /* clear of stream then give a second to slow down before closing stream */
    cout << "Slowing out-stream." << endl;
    ardOut.clear();
    sleep(1);
    ardOut.close();

    /* clear if stream then give a second to slow down before closing stream */
    cout << "Slowing in-stream." << endl;
    ardIn.clear();
    sleep(1);
    ardIn.close();
}

void ArduinoStream::Init() {
    /* use stream openers */
    system(init.c_str()); // c_str() member function takes const char* from string
    system(bufferClear.c_str());
}

void ArduinoStream::Send(string msg) {
    /* wrap msg in system-speak then execute it */
    if(msg.empty())
        return;
    this->previousCommand = "echo " + msg + " | sudo tee --append " + this->serialPort;
    // e.g. `echo move forward | sudo tee --append /dev/ttyACM0` would send 'move forward' to Arduino
    //      all that this relies on is the Arduino being configured to listen to serial comm's
    system(this->previousCommand.c_str());
}

string ArduinoStream::GetPreviousCommand() {
    return this->previousCommand + "\n";
}

#endif // ARDUINOSTREAM_H_INCLUDED
