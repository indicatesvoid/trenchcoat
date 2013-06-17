//
//  Serial.cpp
//  flasharduinomega
//
//  Created by William A. Clark on 6/14/13.
//  Based off of the OpenFrameworks ofSerial class
//

#include "Arduino.h"

using namespace std;

bool Arduino::setup(string port, int baud = 9600) {
    // open the serial device
    fd = open(ports[0].c_str(), O_RDWR | O_NONBLOCK);
    
    if(fd == -1){
        std::cout << "Unable to open port\n";
        return false;
    }
    
    // set baud rate
    struct termios ComParams;
    tcgetattr(fd, &ComParams);
    ComParams.c_cflag &= baud;
    ComParams.c_cflag |= B9600;
    tcsetattr( fd, TCSANOW, &ComParams );
    
    std::cout << "Success in opening serial port\n";
    return true;
}

void Arduino::makeConnection() {
    // modified from code by Stephen Braitsch
    // https://github.com/braitsch
    
    // detect available serial ports //
    string arduinoPort = "";
    buildDeviceList();
    for (int i=0; i < devices.size(); i++) {
        string n = devices[i];
        if (std::string::npos != n.find("/dev/tty.usbserial")) ports.push_back(n);      // for boards that use FTDI serial
        if(std::string::npos != n.find("/dev/tty.usbmodem")) ports.push_back(n);        // for newer boards (Mega, Uno, etc)
    }
    
    if (ports.size() != 0) {
        serialReady = setup(ports[0], 9600);
    }
    
    if (serialReady){
        std::cout << "Arduino found at " << ports[0] << "\n";
    }   else{
        std::cout << "Arduino not found\n";
    }
}

string Arduino::getArduinoPath() {
    return(ports[0]);
}

void Arduino::resetArduino() {
    // set a high bit over the serial line, wait, then reset
    // this will smack the Arduino into resetting
    std::cout<<"Resetting Arduino..." << std::endl;
    ioctl(fd, TIOCMBIS, TIOCM_DTR);
    usleep(500);
    ioctl(fd, TIOCMBIC, TIOCM_DTR);
}

void Arduino::flash(string board, string hexPath) {
    makeConnection();
    resetArduino();
    string cmd = getAvrCommand(board, hexPath);
    system(cmd.c_str());
}

void Arduino::listSupportedBoards() {
    for(int i = 0; i < SUPPORTED_BOARDS; i++) {
        std::string index = std::to_string(i);
        std::cout << boards[i].name << "\n";
        //std::cout << "[" + index + "] " << boards[i].name;
    }
    std::cout << "\n";
}

string Arduino::getAvrCommand(string board, string hexPath) {
    std::string boardCmd = findBoardCommandByName(board);
    std::cout << "Flashing...";
    string cmd = "./avrdude -C avrdude.conf -p" + boardCmd + " -c wiring -P" + ports[0] + " -b 115200 -D -U flash:w:" + hexPath;
    return cmd;
}

string Arduino::findBoardCommandByName(string name) {
    for(int i = 0; i < SUPPORTED_BOARDS; i++) {
        if(boards[i].name == name) {
            return boards[i].cmd;
        }
    }
    return "BOARD_NOT_FOUND";
}

void Arduino::buildDeviceList() {
    // mostly nabbed from ofSerial class
    vector <string> prefixMatch;
    prefixMatch.push_back("cu.");
    prefixMatch.push_back("tty.");
    
    DIR *dir;
	struct dirent *entry;
	dir = opendir("/dev");
    
	string deviceName	= "";
    
	if (dir == NULL){
        std::cout << "Error listing devices in /dev";
	} else {
		//for each device
		while((entry = readdir(dir)) != NULL){
			deviceName = (char *)entry->d_name;
            
			//we go through the prefixes
			for(int k = 0; k < (int)prefixMatch.size(); k++){
				//if the device name is longer than the prefix
				if( deviceName.size() > prefixMatch[k].size() ){
					//do they match ?
					if( deviceName.substr(0, prefixMatch[k].size()) == prefixMatch[k].c_str() ){
						devices.push_back("/dev/"+deviceName);
						break;
					}
				}
			}
		}
		closedir(dir);
	}
}