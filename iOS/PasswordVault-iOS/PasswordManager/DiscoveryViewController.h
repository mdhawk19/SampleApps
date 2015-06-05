//
//  DiscoveryViewController.h
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

/**
 \brief 
 Actual initilization of NCL library followed by discovering the near by Nymi devices will be done here.
 NCLWrapper calss acts as a communication between app and the NCL library. Its a wrapper class written in Objective C to communicate with NCL library. 
 
 Start discovering the Nymi band by calling "discoverNymiWithHandler". Internally it will initlize NCL library and upon successful initialization it will start discovering Nymi bands near by. The nearest Nymi will be picked and gets the LEDs status. Response from this method indicates if its successful in getting LEDs status or failed to discover.
 
 If it successfully discovers Nymi band, it will take the user to "Comfirmation" screen where it will display the signature of LEDs On/Off. Users can confim the LEDs on Nymi band and LEDs in the screen, then "Accept" or "Reject" accordingly.
 */
#import <UIKit/UIKit.h>

@interface DiscoveryViewController : UIViewController

@end

