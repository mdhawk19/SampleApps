//
//  WarningViewController.h
//  PasswordVault
//
//  Created by Satyanarayana on 19/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>


/**
 \brief The WarningViewController class is generic custom class with shows a alertview.
 
 This class instance can be presented as a overlay view to underlaying parent view.This class has two properties namely warningTitle and warningMessage, using which the custom alert view is displayed to the user.
 */

@interface WarningViewController : UIViewController

@property (nonatomic, strong) NSString* warningTitle;
@property (nonatomic, strong) NSString* warningMessage;

@end
