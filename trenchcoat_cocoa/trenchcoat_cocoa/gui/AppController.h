//
//  AppController.h
//  trenchcoat_cocoa
//
//  Created by William A. Clark on 6/18/13.
//  Copyright (c) 2013 William A. Clark. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <vector>
#import <string>
#import "Arduino.h"

@interface AppController : NSObject {
@private
    Arduino *arduino;
    std::string *selectedBoard;
    std::string *file;
    NSArray* supportedBoards;
    IBOutlet NSTextField *fileField;
    IBOutlet NSPopUpButton *boardDropdown;
}

- (NSArray*)stringVectorToNSArray:(std::vector<std::string>)vector;
- (IBAction)browseFiles:(id)sender;
- (IBAction)flash:(id)sender;


@end
