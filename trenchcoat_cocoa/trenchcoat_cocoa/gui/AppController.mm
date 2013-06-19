//
//  AppController.m
//  trenchcoat_cocoa
//
//  Created by William A. Clark on 6/18/13.
//  Copyright (c) 2013 William A. Clark. All rights reserved.
//

#import "AppController.h"
#import "Arduino.h"

@implementation AppController
- (id)init {
    if(self = [super init]) {
        arduino = new Arduino();
    }
    return self;
}

-(void)awakeFromNib {
    supportedBoards = [self stringVectorToNSArray:arduino->returnSupportedBoards()];
    [boardDropdown addItemsWithTitles:supportedBoards];
    selectedBoard = new std::string([[boardDropdown titleOfSelectedItem] UTF8String]);
}

- (IBAction)onSelectBoard:(id)sender {
    selectedBoard = new std::string([[boardDropdown titleOfSelectedItem] UTF8String]);
}

- (IBAction)browseFiles:(id)sender {
    // Create a File Open Dialog class.
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    // Set array of file types
    NSArray *fileTypesArray;
    fileTypesArray = [NSArray arrayWithObjects:@"hex", nil];
    
    // Enable options in the dialog.
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowedFileTypes:fileTypesArray];
    [openDlg setAllowsMultipleSelection:FALSE];
    
    // Display the dialog box.  If the OK pressed,
    // process the files.
    if ( [openDlg runModal] == NSOKButton ) {
        
        // Gets list of all files selected
        //NSArray *files = [openDlg URLs];
        NSString *url = [[openDlg URL] path];
        
        NSLog(@"File path: %@", url);
        [fileField setStringValue:url];
        file = new std::string([url UTF8String]);
    }
}

- (IBAction)flash:(id)sender {
    if(*file != "") {
        arduino->flash(*selectedBoard, *file);
    }
}

- (id)stringVectorToNSArray:(std::vector<std::string>)vector {
    id nsstrings = [NSMutableArray new];
    std::for_each(vector.begin(), vector.end(),
                  [&nsstrings](std::string str) {
                      id nsstr = [NSString stringWithUTF8String:str.c_str()];
                      [nsstrings addObject:nsstr];
                  });
    return nsstrings;
}

@end
