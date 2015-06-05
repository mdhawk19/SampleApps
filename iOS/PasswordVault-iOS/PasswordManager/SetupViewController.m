//
//  SetupViewController.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "SetupViewController.h"
#import "Utility.h"
#import "Constants.h"

@interface SetupViewController () <UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UIButton *setupButton;

@end

@implementation SetupViewController


/**
 This method is part of view's life cycle. It will be called as soon as controller's view is loaded into memory. This method will be called regardless of wheter view is loaded from XIB or storyboard or created programatically. You usually override this method to perform additional initialization on views that were loaded from nib files.
 
 Style of View & "Begin Set Up" button are customized.
 */
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.view.backgroundColor = [Utility colorFromHexString:DARK_NAVY];
    self.setupButton.backgroundColor = [Utility colorFromHexString:ORANGE_NORMALSATE];
    self.setupButton.layer.cornerRadius = 3;
}

/**
 When the app receives memory warning, this notification will be sent to view controller. You should never call this method directly. When the system determines that the app is occupying more memory than allocated, it will throw a notification "available memory is low"
 
 In this method you can release any of the objects that are taking more memory like arrays etc.
 */
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
