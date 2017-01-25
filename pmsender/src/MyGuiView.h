//
//  MyGuiView.h
//  pmApp_sender
//
//  Created by kayayose on 2014/06/04.
//
//

#import <UIKit/UIKit.h>
#include "ofApp.h"

@interface MyGuiView : UIViewController{
    ofApp *myApp;
    IBOutlet UISegmentedControl *mySegmentedControl;
}

- (IBAction)inputtext:(id)sender;
- (IBAction)SegChanged:(id)sender;
- (NSString *)deviceIPAdress;

#define MAXADDRS	32

extern char *if_names[MAXADDRS];
extern char *ip_names[MAXADDRS];
extern char *hw_addrs[MAXADDRS];
extern unsigned long ip_addrs[MAXADDRS];

// Function prototypes

void InitAddresses();
void FreeAddresses();
void GetIPAddresses();
void GetHWAddresses();


@end
