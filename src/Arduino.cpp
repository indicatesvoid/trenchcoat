//
//  Arduino.cpp
//  Trenchcoat
//
//  Created by William A. Clark on 6/14/13.
//  Based off of the OpenFrameworks ofSerial class
//

#include "Arduino.h"

using namespace std;

//////////////////////////////////////////////
//                                          //
//              PUBLIC METHODS              //
//                                          //
//////////////////////////////////////////////

Arduino::Arduino() {
    devices = buildDeviceList();
    arduinos = findArduinos();
}

bool Arduino::connect(int baud = 9600) {
    if(arduinos.size() == 0) cout << "No Arduinos found. Please plug one in and run the application again." << endl;
    else if(arduinos.size() > 1) device_path = chooseActiveDevice();
    else device_path = arduinos[0];
    
    // open the serial device
    fd = open(device_path.c_str(), O_RDWR | O_NONBLOCK);
    
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

void Arduino::resetArduino() {
    // set a high bit over the serial line, wait, then reset
    // this will smack the Arduino into resetting
    std::cout<<"Resetting Arduino..." << std::endl;
    ioctl(fd, TIOCMBIS, TIOCM_DTR);
    usleep(500);
    ioctl(fd, TIOCMBIC, TIOCM_DTR);
}

void Arduino::flash(string board, string hexPath) {
    findArduinos();
    resetArduino();
    string cmd = getAvrCommand(board, hexPath);
    system(cmd.c_str());
}

void Arduino::listDetectedDevices() {
    for(int i = 0; i < arduinos.size(); i++) {
        string index = to_string(i);
        cout << "[" << index << "] " << arduinos[i] << endl;
    }
}

void Arduino::listSupportedBoards() {
    for(int i = 0; i < SUPPORTED_BOARDS; i++) {
        std::string board_name = boards[i].name;
        board_name[0] = toupper(board_name[0]);
        std::cout << "  " << boards[i].name << "\n";
    }
    std::cout << "\n";
}

//////////////////////////////////////////////
//                                          //
//              PRIVATE METHODS             //
//                                          //
//////////////////////////////////////////////

vector<string> Arduino::findArduinos() {
    // modified from code by Stephen Braitsch
    // https://github.com/braitsch
    
    // detect available serial ports //
    std::vector<std::string> _arduinos;
    string arduinoPort = "";
    for (int i=0; i < devices.size(); i++) {
        string n = devices[i];
        if (std::string::npos != n.find("/dev/tty.usbserial")) _arduinos.push_back(n);      // for boards that use FTDI serial
        if(std::string::npos != n.find("/dev/tty.usbmodem")) _arduinos.push_back(n);        // for newer boards (Mega, Uno, etc)
    }
    
    return _arduinos;
}

string Arduino::chooseActiveDevice() {
    cout << "Multiple Arduinos detected:/n";
    listDetectedDevices();
    cout << "Please choose active device: ";
    string input;
    int deviceIndex;
    bool input_is_int = false;
    bool input_is_valid = false;
    cin >> input;
    
    // test if input is a valid integer
    std::string::const_iterator it = input.begin();
    while (it != input.end() && std::isdigit(*it)) ++it;
    if(!input.empty() && it == input.end()) input_is_int = true;
    
    if(input_is_int) {
        // user passed device index as input
        deviceIndex = stoi(input);
        // test if user input is valid
        if(deviceIndex < arduinos.size() + 1) input_is_valid = true;
    }
    
    else {
        // test if user input is valid path
        for(int i = 0; i < arduinos.size(); i++) {
            if(input == arduinos[i]) {
                input_is_valid = true;
                break;
            }
        }
    }
    
    if(input_is_valid) {
        if(input_is_int) return arduinos[deviceIndex];
        else return input;
    }
    
    else {
        cout << "Selection invalid. Please make sure input is a valid port index or path.";
        chooseActiveDevice();
    }
    
    return arduinos[0]; // default/fallback, just in case.
}

string Arduino::getAvrCommand(string board, string hexPath) {
    std::string boardCmd = findBoardCommandByName(board);
    std::cout << "Flashing...";
    string cmd = "./avrdude -C avrdude.conf -p" + boardCmd + " -c wiring -P" + device_path + " -b 115200 -D -U flash:w:" + hexPath;
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

vector<string> Arduino::buildDeviceList() {
    // mostly nabbed from ofSerial class
    vector <string> prefixMatch;
    prefixMatch.push_back("cu.");
    prefixMatch.push_back("tty.");
    
    vector<string> _devices;
    
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
						_devices.push_back("/dev/"+deviceName);
						break;
					}
				}
			}
		}
		closedir(dir);
	}
    
    return _devices;
}