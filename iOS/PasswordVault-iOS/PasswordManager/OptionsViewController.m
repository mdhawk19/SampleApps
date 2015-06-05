//
//  OptionsViewController.m
//  PasswordVault
//
//  Created by Satyanarayana on 01/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "OptionsViewController.h"
#import "AlertsViewController.h"
#import "PMAlertView.h"
#import "Constants.h"
#import "PasswordManager.h"
#import "Utility.h"

@implementation OptionsViewController

-(void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor colorWithPatternImage:TRANSPARENT_IMAGE];
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] init];
    [tapGesture addTarget:self action:@selector(closeAction)];
    [self.view addGestureRecognizer:tapGesture];
}
/**
 This method will be used when you use "storyboard" in your application. As and when a new view controller is going to be presented, system will notify the view controller about the new "segue" to be performed.
 
 You have to over ride this method when you have to pass some data to destination view controller that is going to be displayed.
 
 Set the alert type for destination view controller that displays appropriate data.
 
 @param segue The segue object containing information about the view controllers involved in the segue.
 @param sender The object that initiated the segue. You might use this parameter to perform different actions based on which control (or other object) initiated the segue.
 */
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    AlertsViewController* alertsVC = segue.destinationViewController;
    if ([segue.identifier isEqualToString:COPY_TO_CLIPBOARD_SEGUE]) {
        alertsVC.alertViewType = PMAlertTypeCopyPassword;
    } else if ([segue.identifier isEqualToString:VIEW_PASSWORD_SEGUE]) {
        alertsVC.alertViewType = PMAlertTypeViewPassword;
    } else if ([segue.identifier isEqualToString:CHANGE_PASSWORD_SEGUE]) {
        alertsVC.alertViewType = PMAlertTypeChangePassword;
    } else if ([segue.identifier isEqualToString:REMOVE_SEGUE]) {
        alertsVC.alertViewType = PMAlertTypeRemovePassword;
    }
}

/**
 This method allows the user to close the options view.
*/
- (void)closeAction {
    [self.delegate hideOptions];
}


- (IBAction)closeButtonTouched:(id)isender{
    [self.delegate hideOptions];
}

/**
 When user selects Launch URl option, this method gives the callback to the MainViewController to validate the URl and continue with the action.
 */
- (IBAction)showURL:(id)sender {
    [self.delegate openLabelURL];
}

@end
