//
//  WelcomeViewController.h
//  PasswordVault
//
//  Created by Satyanarayana on 16/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

/**
 \brief
 This is the initial view controller that will be displayed to user immediately after splash screen. This is also used as loading view which makes a check to see if any Nymi band is already provisioned. If no Nymi band is provisioned earlier, it will show the Setup screen, else, it will try to validate the Nymi band.
 
 @see SetupViewController, ValidateViewController
 */

@interface WelcomeViewController : UIViewController

@end
