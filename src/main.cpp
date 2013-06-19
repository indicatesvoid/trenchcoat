//
//  main.cpp
//  Trenchcoat
//
//  Created by William A. Clark on 6/14/13.
//

#include "main.h"

using namespace std;

int main(int argc, const char * argv[])
{
    ez::ezOptionParser opt;
    
    opt.overview = "Simple program to flash a HEX file to an Arduino from the commandline.";
    opt.syntax = "trenchcoat [OPTIONS]";
    opt.example = "trenchcoat -b mega2560 -f /path/to/file.hex\n\n";
    
    opt.add(
            "", // Default.
            0, // Required?
            0, // Number of args expected.
            0, // Delimiter if expecting multiple args.
            "Display usage instructions.", // Help description.
            "-h",     // Flag token.
            "-help",  // Flag token.
            "--help", // Flag token.
            "--usage" // Flag token.
            );
    
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
    
    if (opt.isSet("-h")) {
        std::string usage;
        opt.getUsage(usage);
        std::cout << usage;
        return 1;
    }
    
    if(!opt.isSet("-b")) {
        std::cout << "Please define Arduino type. Supported types:\n";
        arduino->listSupportedBoards();
    }
    
    if(!opt.isSet("-f")) {
        std::cout << "Please define a path to a valid hex file, e.g. 'hexduino -b mega2560 -f /path/to/file.hex'" << endl;
    }
    
    if(opt.isSet("-b") && opt.isSet("-f")) {
        opt.get("-b")->getString(board);
        opt.get("-f")->getString(file);
        arduino->connect(9600);
        arduino->flash(board, file);
    }
    
    return 0;
}


