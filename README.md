hexduino
========

Simple application using avrdude to flash a HEX file to an Arduino over the command line.

### Dependencies and Requirements

Currently built for Mac OS. Should compile against most (if not all) recent versions of OSX.

As of now, the application requires the avrdude binary and associated conf file (included in the 'build' folder) present in the same folder as the compiled binary in order to run. Hoping to make this a bit more compact and bundled at a later date, but for now, it works.

### Usage

Plug in 'yerr board, and run:

hexduino -b BOARD_NAME -f /path/to/file.hex

Currently supports:
####Mega 2560 
-b mega2560

####Uno
-b uno
(UNTESTED)

####Leonardo
-b leonardo
(UNTESTED)

