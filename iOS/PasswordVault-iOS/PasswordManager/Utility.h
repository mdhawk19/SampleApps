//
//  Utility.h
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/23/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

@import UIKit;
#import "Constants.h"

/**
 \brief The Utility class is a sub-class on NSObject, which provides with all the helper methods which can be accessed from other classes.
 
 Utility class is a generic class which facilitate to declare methods which are required by many other classes.
 */
@interface Utility : NSObject

/**
 \brief The static method colorFromHexString is a function which generates UIColor object by taking hexString as an input. 
 
 This method is usefull in getting Background colors of the views, button and other UI elements.
 
 @param hexString a string object which holds the RGB value in hex.
 @return \brief returns UIColor object
 
 This function returns an object of UIColor which can be used as a value to set Background colors of the views, button and other UI elements
 */
+ (UIColor*)colorFromHexString:(NSString *)hexString;


/**
 \brief The static method showAlertWithTitle is a function which creates alertviews by taking title and message strings as inputs.
 
 This is a generic method which shows specific messages to the user by recieving title and messages as inputs.
 
 @param title a string object which holds title of the alertview shown.
 @param message a string object which holds body or the message which needs to be shown in the alertview.
 
 */
+ (void)showAlertWithTitle:(NSString*)title andMessage:(NSString*)message;
@end
