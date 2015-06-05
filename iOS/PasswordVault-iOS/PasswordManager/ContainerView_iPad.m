//
//  ContainerView_iPad.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 21/01/15.
//  Copyright (c) 2015 HappiestMinds. All rights reserved.
//

#import "ContainerView_iPad.h"
#import "MenuViewController.h"
#import "MainViewController.h"
#import "Constants.h"
#import "AlertsViewController.h"

@interface ContainerView_iPad ()

@property (nonatomic, strong) MenuViewController *menuController;
@property (nonatomic, strong) MainViewController *mainController;

@end

@implementation ContainerView_iPad

/**
 When the app receives memory warning, this notification will be sent to view controller. You should never call this method directly. When the system determines that the app is occupying more memory than allocated, it will throw a notification "available memory is low"
 
 In this method you can release any of the objects that are taking more memory like arrays etc.
 */
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


/**
 This method will be used when you use "storyboard" in your application. As and when a new view controller is going to be presented, system will notify the view controller about the new "segue" to be performed.
 
 You have to over ride this method when you have to pass some data to destination view controller that is going to be displayed.
 
 
 Here in this viewcontroller we are handling two segues which present the Menu View and Listing View side by side. To get the instances of MenuView class and ListingView class we are overriding this method and we are setting MenuViews delegate as ListingView to handle functionalities from the MenuView.
 
 @param segue The segue object containing information about the view controllers involved in the segue.
 @param sender The object that initiated the segue. You might use this parameter to perform different actions based on which control (or other object) initiated the segue.
 */
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([segue.identifier isEqualToString:@"MenuView"]) {
        // can't assign the view controller from an embed segue via the storyboard, so capture here
        self.menuController = (MenuViewController *)segue.destinationViewController;
    } else if ([segue.identifier isEqualToString:@"MainView"]) {
        // can't assign the view controller from an embed segue via the storyboard, so capture here
        self.mainController = (MainViewController *)segue.destinationViewController;
    }
    
    if (self.menuController != nil && self.mainController != nil) {
        self.menuController.delegate = self.mainController;
    }
}

@end
