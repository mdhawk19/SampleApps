//
//  ConfirmViewController.h
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//


#import <UIKit/UIKit.h>

/**
 \brief 
 This view displays the status of LEDs received from discovered Nymi. Users has to manually verify the LEDs on their Nymi band and check with the signature of LED images high lighted. If they are matching, it means the device they want to provision is the right one that will be provisioned. Else, they have to start discovering again.
 
 Accordingly, user will be taken to Validation screen
 */
@interface ConfirmViewController : UIViewController

/**
 The agree code received from previous view will be passed before it will be diplayed to user. While rendering UI, agree code will be parsed and LEDs are set On/Off accordingly
 */
@property (nonatomic,retain) NSString *agreeCode;


@end
