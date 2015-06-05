//
//  WelcomeViewController.m
//  PasswordVault
//
//  Created by Satyanarayana on 16/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "WelcomeViewController.h"
#import "Utility.h"

@implementation WelcomeViewController


/**
 This method is part of view's life cycle. Notifies the view controller that its view was added to a view hierarchy. One can over ride this method to handle the tasks (like updating the UI etc) associated while showing the view. Its mandatory to call "super" when over riding.
 
 Here, we are showing the view for 1 second, and then we will check if a Nymi band already provisioned or not.
 
 @parm animated If YES, the view was added to the window using an animation.
 */
-(void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    self.view.backgroundColor = [Utility colorFromHexString:DARK_NAVY];
    [self performSelector:@selector(showView) withObject:nil afterDelay:LOADING_DELAY];
}


/**
 showView will check for provisioned. If it finds a provisioned Nymi band, it take the user for validating it. Else, it will give option for the user to provision a Nymi band.
 
 When a Nymi band is provisioned, the ID generated will be stored in NSUserDefaults and the same will be used to verify if a Nymi band is already provisioned or not.
 
 If Nymi band is provisioned, it will take the user to Validation screen else, Set up screen will be displayed.
 */
-(void)showView {
    NSString* provisionID = [[NSUserDefaults standardUserDefaults] valueForKey:PROV_ID];
    if (!provisionID || !provisionID.length) {
        [self performSegueWithIdentifier:INITIAL_SETUP_SEGUE sender:nil];
    } else {
        [self performSegueWithIdentifier:INITIAL_VALIDATION_SEGUE sender:nil];
    }
}

@end
