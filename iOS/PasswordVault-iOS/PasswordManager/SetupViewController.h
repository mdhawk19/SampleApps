//
//  SetupViewController.h
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
/**
  \brief 
 In order to use Nymi band with the app, it has to be provisioned with the application. This screen is the starting point to initiate different steps to provision a Nymi bank.
 
 First step of provisioning is to initialize the Nymi SDK (library), then start discovering the nearest device, and then get the LEDs status and sync with the device for confimation.
 
 Just tap on "Begin Set Up" button to start provisioning
 */
@interface SetupViewController : UIViewController


@end

