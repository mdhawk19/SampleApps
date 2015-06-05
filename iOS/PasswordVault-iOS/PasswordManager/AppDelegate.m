//
//  AppDelegate.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "AppDelegate.h"
#import "Constants.h"

@implementation AppDelegate


/**
 This delegate method will be called when the app completes the launch process and is ready to run
 @param application Singleton object of "app"
 @param launchOptions Its a dictionary of options provided by UIApplication that are enabled the app to launch. This will be nil/empty when the app is directly launched by user by tapping on app "Icon". To know more about launch options please visit <a href='#//apple_ref/doc/constant_group/Launch_Options_Keys'>Launch Options Keys</a>
 @returns NO if the app cannot handle the URL resource or continue a user activity, otherwise return YES. The return value is ignored if the app is launched as a result of a remote notification.
 @see applicationWillEnterForeground:
 */

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [[NSUserDefaults standardUserDefaults] registerDefaults:@{PROV_ID : @""}];
    [[UIApplication sharedApplication] setStatusBarHidden:NO];
    return YES;
}


/**
 This delegate method will be called when the app is about to enter foreground when the transition of the app is from background state to active state. 
 @param application Singleton object of "app"
 @see application:didFinishLaunchingWithOptions:
 */
- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    UINavigationController* navController = (UINavigationController *)self.window.rootViewController;
    [navController popToRootViewControllerAnimated:NO];
}

@end
