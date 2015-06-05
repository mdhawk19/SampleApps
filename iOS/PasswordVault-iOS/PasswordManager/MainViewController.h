//
//  MainViewController.h
//  PasswordVault
//
//  Created by Satyanarayana on 30/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MenuViewController.h"


/**
 \brief The MainViewController class presents the Listing of all the Label which are added by the user This viewcontroller acts as a delegate to MenuView and Recieves all the calls related to Functionalities listed in the MenuView.
 
 This viewController acts as the Home page of the application by driving all user actions like Add password, show passwords,Delete Password and Unauthorize User with the help of protocol MenuDelegate.
 */
@interface MainViewController : UIViewController <MenuDelegate>

@end
