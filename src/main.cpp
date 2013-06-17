//
//  main.cpp
//  Hexduino
//
//  Created by William A. Clark on 6/14/13.
//

#include "main.h"

using namespace std;

int main(int argc, const char * argv[])
{
    ez::ezOptionParser opt;
    
    opt.overview = "Simple program to flash a HEX file to an Arduino from the commandline.";
    opt.syntax = "hexduino [OPTIONS]";
    opt.example = "hexduino -b mega2560 -f /path/to/file.hex\n\n";
    
    opt.add(
            "", // Default.
            1, // Required?
            1, // Number of args expected.
            0, // Delimiter if expecting multiple args.
            "Define board type to flash to.", // Help description.
            "-b",     // Flag token.
            "-board"  // Flag token.
            );
    
    opt.add(
            "", // Default.
            1, // Required?
            1, // Number of args expected.
            0, // Delimiter if expecting multiple args.
            "Define HEX file to flash.", // Help description.
            "-f",     // Flag token.
            "-file"  // Flag token.
            );
    
    opt.parse(argc, argv);
    
    if(!opt.isSet("-b")) {
        std::cout << "Please define Arduino type. Supported types:\n";
        arduino->listSupportedBoards();
    }
    
    if(!opt.isSet("-f")) {
        std::cout << "Please define a path to a valid hex file, e.g. 'hexduino -b mega2560 -f /path/to/file.hex'";
    }
    
    if(opt.isSet("-b") && opt.isSet("-f")) {
        opt.get("-b")->getString(board);
        opt.get("-f")->getString(file);
        arduino->flash(board, file);
    }
    
    return 0;
}


