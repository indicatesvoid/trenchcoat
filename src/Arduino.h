//
//  Arduino.h
//  Trenchcoat
//
//  Created by William A. Clark on 6/14/13.
//  Do as ye please.
//

#include <vector>
#include <string>
#include <map>

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <dirent.h>

class Arduino {
    public:
        Arduino();
    
        bool connect(int baud);
        void resetArduino();
        void flash(std::string board, std::string hexPath);
        void listSupportedBoards();
    
    private:

    // METHODS

        std::vector<std::string> findArduinos();
        std::vector<std::string> buildDeviceList();
        void listDetectedDevices();
        std::string chooseActiveDevice();
        std::string getAvrCommand(std::string board, std::string hexPath);
        std::string findBoardCommandByName(std::string name);
    

    // VARIABLES

        std::string device_path;
        int fd;
        std::vector<std::string> devices;
        std::vector<std::string> arduinos;
    
        struct Board {
            std::string name;
            std::string cmd;
        };
    
        static const int SUPPORTED_BOARDS = 4;
    
        // could use std::map for this
        // leaving it as a struct for now for
        // later flexibility
        Board boards[SUPPORTED_BOARDS] = {
            {"mega2560", "atmega2560"},
            {"uno", "m328p" },
            {"leonardo", "m32u4" },
            {"duemilanove", "m328p" }
        };
    
        struct Device {
            int index;
            std::string path;
        };
};


