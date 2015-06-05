//
//  WarningViewController.m
//  PasswordVault
//
//  Created by Satyanarayana on 19/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "WarningViewController.h"
#import "PMButton.h"

@interface WarningViewController ()

@property (weak, nonatomic) IBOutlet UILabel *topTitle;
@property (weak, nonatomic) IBOutlet UILabel *message;
@property (weak, nonatomic) IBOutlet PMButton *okButton;

@end

@implementation WarningViewController

/**
 When the app receives memory warning, this notification will be sent to view controller. You should never call this method directly. When the system determines that the app is occupying more memory than allocated, it will throw a notification "available memory is low"
 
 In this method you can release any of the objects that are taking more memory like arrays etc.
 */
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];
    self.topTitle.text = self.warningTitle;
    self.message.text = self.warningMessage;
}

- (IBAction)close:(id)sender {
    [self.view removeFromSuperview];    
    [self removeFromParentViewController];
}


@end
