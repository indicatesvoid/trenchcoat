//
//  Arduino.h
//  Hexduino
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
        struct Board {
            std::string name;
            std::string cmd;
        };
        static const int SUPPORTED_BOARDS = 4;
    /*
    std::map < std::string, std::string > boards = {
        {"mega2560", "atmega2560"},
        {"uno", "m328p" },
        {"leonardo", "m32u4" },
        {"duemilanove", "m328p" }
    };
     */
    
        
        Board boards[SUPPORTED_BOARDS] = {
            {"mega2560", "atmega2560"},
            {"uno", "m328p" },
            {"leonardo", "m32u4" },
            {"duemilanove", "m328p" }
        };
                     
        void makeConnection();
        void resetArduino();
        void flash(std::string board, std::string hexPath);
        std::string getArduinoPath();
        void listSupportedBoards();
        
    
    private:
        
        bool setup(std::string port, int baud);
        void buildDeviceList();
    
        std::string getAvrCommand(std::string board, std::string hexPath);
        std::string findBoardCommandByName(std::string name);
    
        bool serialReady;
        int fd;
        std::vector<std::string> devices;
        std::vector<std::string> ports;
};


