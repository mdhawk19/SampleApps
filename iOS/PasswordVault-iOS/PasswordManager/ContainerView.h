//
//  ContainerView.h
//  PasswordVault
//
//  Created by Satyanarayana on 31/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "PKRevealController.h"

/**
 \brief 
 PKRevealController is a 3rd party code that will provide sliding menu animation based on gesture.
 Container View class will just hold two view controllers viz Menu and Listing screens that provide necessary functionality to handle Menu and Listing of Labels respectively.
 Swipe from left to right to reveal the menu and swipe from right to left to hide the menu. Performing any action on menu also hides it and relative action will be performed.
 */
@interface ContainerView : PKRevealController


@end
