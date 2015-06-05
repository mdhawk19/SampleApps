//
//  Utility.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/23/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "Utility.h"
#import "WarningViewController.h"

@implementation Utility

/**
 \brief The static method colorFromHexString is a function which generates UIColor object by taking hexString as an input.
 
 This method is usefull in getting Background colors of the views, button and other UI elements.
 
 @param hexString a string object which holds the RGB value in hex.
 @return \brief returns UIColor object
 
 This function returns an object of UIColor which can be used as a value to set Background colors of the views, button and other UI elements
 */
+ (UIColor *)colorFromHexString:(NSString *)hexString {
    unsigned rgbValue = 0;
    NSScanner *scanner = [NSScanner scannerWithString:hexString];
    [scanner setScanLocation:1]; // bypass '#' character
    [scanner scanHexInt:&rgbValue];
    return [UIColor colorWithRed:((rgbValue & 0xFF0000) >> 16)/255.0 green:((rgbValue & 0xFF00) >> 8)/255.0 blue:(rgbValue & 0xFF)/255.0 alpha:1.0];
}

/**
 \brief The static method showAlertWithTitle is a function which creates alertviews by taking title and message strings as inputs.
 
 This is a generic method which shows specific messages to the user by recieving title and messages as inputs.
 
 @param title a string object which holds title of the alertview shown.
 @param message a string object which holds body or the message which needs to be shown in the alertview.
 
 */
+ (void)showAlertWithTitle:(NSString*)title andMessage:(NSString*)message {
    UIStoryboard* mainStoryboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    WarningViewController* warningVC = [mainStoryboard instantiateViewControllerWithIdentifier:@"WARNINGVIEW"];
    warningVC.warningTitle = title;
    warningVC.warningMessage = message;
    
    UINavigationController* navController = (UINavigationController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    UIViewController *rootViewController = [navController visibleViewController];
    [rootViewController addChildViewController:warningVC];
    
    warningVC.view.frame = rootViewController.view.bounds;
    [rootViewController.view addSubview:warningVC.view];
    [warningVC didMoveToParentViewController:rootViewController];
}

@end
