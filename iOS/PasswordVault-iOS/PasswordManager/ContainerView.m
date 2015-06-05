//
//  ContainerView.m
//  PasswordVault
//
//  Created by Satyanarayana on 31/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "ContainerView.h"
#import "MenuViewController.h"
#import "MainViewController.h"
#import "Constants.h"

@interface ContainerView ()
@property(nonatomic, strong) MenuViewController* menu;
@property(nonatomic, strong) MainViewController* mainView;
@end

@implementation ContainerView

/**
 This method is part of view's life cycle. It will be called as soon as controller's view is loaded into memory. This method will be called regardless of wheter view is loaded from XIB or storyboard or created programatically. You usually override this method to perform additional initialization on views that were loaded from nib files.
 
 This is the container that holds Sliding Menu and Labels listing. Menu will be sliding from left to right upon swipe gesture. Accordingly it will hide.
 
 Create Menu and Main view controllers and add to the container. 
 */
-(void)viewDidLoad {
    [super viewDidLoad];
    self.mainView = [self.storyboard instantiateViewControllerWithIdentifier:MAINVIEW_SEGUE];
    self.menu = [self.storyboard instantiateViewControllerWithIdentifier:MENUVIEW_SEGUE];
    self.menu.delegate = self.mainView;
    
    self.frontViewController = self.mainView;
    self.leftViewController = self.menu;
    self.rightViewController = nil;
}

- (void)showViewController:(UIViewController *)controller {
    [super showViewController:controller];
}

/**
 This method is part of View's life cycle. As soon as Controller's view is added to view hierarcy, controller will be notified.
 
 You can over ride this method to handle any of the UI customization or any other task associated with it. You must call "super" at some point of implementation.
 
 showViewController is implemented in the parent "PKRevealController" to display default view to be visible to user.
 */
-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self showViewController:self.frontViewController];
}

@end
