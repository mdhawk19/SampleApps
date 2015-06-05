//
//  AppDelegate.h
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

/**
 UIApplicationDelegate is a protocol that defines the methods which are relevant to life cycle of 
 an application. When ever there are relevant changes in the flow of an application, appropriate
 methods will be called. This AppDelegate registers for those delegate methods and works in parallel
 with the app to ensure that the application methods are called properly.
 
 @author Satyanarayana SVV
 */

@interface AppDelegate : UIResponder <UIApplicationDelegate>

/**
 Window is the container to hold the entire application. This property contains the window used to present the app’s visual content on the device’s main screen. Its mandatory to declare this property if "UIMainStoryboardFile" is definined in "Info.plist" file.
 */
@property (strong, nonatomic) UIWindow *window;


@end

