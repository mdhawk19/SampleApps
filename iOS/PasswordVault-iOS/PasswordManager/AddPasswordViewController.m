//
//  AddPasswordViewController.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 09/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "AddPasswordViewController.h"
#import "Constants.h"
#import "PasswordManager.h"
#import "Utility.h"
#import "PMTextField.h"


#define HOLDER_DEFAULT_YPOSITION 70
#define HOLDER_ANIMATE_YPOS_IPHONE -80
#define HOLDER_ANIMATE_YPOS_IPAD -20

@interface AddPasswordViewController () <UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UIView *customNavBarView;
@property (weak, nonatomic) IBOutlet UILabel *firstPasswordLabel;
@property (weak, nonatomic) IBOutlet UITextField *passwordTextField;
@property (weak, nonatomic) IBOutlet UITextField *labelTextField;
@property (weak, nonatomic) IBOutlet PMTextField *website;

@property (weak, nonatomic) IBOutlet UIView *viewsHolder;

@property (weak, nonatomic) IBOutlet NSLayoutConstraint* topConstraintForViewHolder;

@end

@implementation AddPasswordViewController

/**
 This method is part of view's life cycle. It will be called as soon as controller's view is loaded into memory. This method will be called regardless of wheter view is loaded from XIB or storyboard or created programatically. You usually override this method to perform additional initialization on views that were loaded from nib files.
 
 Change the title of the label based on number of tags available
 */
- (void)viewDidLoad{
    [super viewDidLoad];
    
    [self.customNavBarView setBackgroundColor:[Utility colorFromHexString:DARK_NAVY]];

    NSArray *passwordsArray = [PasswordManager getAllTags];
    if ([passwordsArray count] > 0) {
//        self.firstPasswordLabel.text = @"";
        self.firstPasswordLabel.text = NSLocalizedString(@"Your Nymi Band has been authorized. You can now save your password!", @"Your Nymi Band has been authorized. You can now save your password!");
    }
}

-(void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    [self.labelTextField resignFirstResponder];
    [self.passwordTextField resignFirstResponder];
    [self.website resignFirstResponder];
}


/**
 This method is a delegate to UITextFieldDelegate which tells the implementing class that the user has tapped on return button on the keyboard.
 */
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

/**
 This method is a delegate to UITextFieldDelegate which tells the implementing class that the a textfield has become firstresponder.
 */
- (void)textFieldDidBeginEditing:(UITextField *)textField {
    if (IS_IPAD) {
        if (UIDeviceOrientationIsLandscape([[UIApplication sharedApplication] statusBarOrientation])) {
            self.topConstraintForViewHolder.constant = HOLDER_ANIMATE_YPOS_IPAD;
            [self.viewsHolder setNeedsUpdateConstraints];
            [self.viewsHolder layoutIfNeeded];
        }
    } else {
        self.topConstraintForViewHolder.constant = HOLDER_ANIMATE_YPOS_IPHONE;
        [self.viewsHolder setNeedsUpdateConstraints];
        [self.viewsHolder layoutIfNeeded];
    }
}

/**
 This method is a delegate to UITextFieldDelegate which tells the implementing class that the user has tapped on return button on the keyboard.
 */
- (void)textFieldDidEndEditing:(UITextField *)textField {
    self.topConstraintForViewHolder.constant = HOLDER_DEFAULT_YPOSITION;
    [self.viewsHolder setNeedsUpdateConstraints];
    [self.viewsHolder layoutIfNeeded];
}


/**
 In the Add password page when user clicks cancel button this method will get called. This method takes care of closing the view along with dismissing of the keyboard which is presented while showing the view.
 */
- (IBAction)cancelAction:(id)sender {
    [self.labelTextField resignFirstResponder];
    [self.passwordTextField resignFirstResponder];
    [self.website resignFirstResponder];
    [self.navigationController popViewControllerAnimated:YES];
}

/**
 This method adds the new password to the database by doing required field and lenght validations.
 */
- (IBAction)addAction:(id)sender {
    [self.labelTextField resignFirstResponder];
    [self.passwordTextField resignFirstResponder];
    [self.website resignFirstResponder];
    
    NSString *tagValue = self.labelTextField.text;
    NSString *passwordValue = self.passwordTextField.text;
    NSString *websiteURL = self.website.text;
    
    if (tagValue.length == 0) {
        [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!") andMessage:NSLocalizedString(@"Label cannot be empty", @"Label cannot be empty")];
        return;
    }
    
    if (passwordValue.length == 0) {
        [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!") andMessage:NSLocalizedString(@"Password cannot be empty", @"Password cannot be empty")];
        return;
    }
    
    if (tagValue.length > MAXLENGTH_PASSWORD) {
        [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!") andMessage:[NSString stringWithFormat:NSLocalizedString(@"Label/Tag name should allow up to 50 characters", @"Label/Tag name should allow up to 50 characters"), tagValue]];
        return;
    }
    
    if (passwordValue.length > MAXLENGTH_PASSWORD) {
        [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!") andMessage:[NSString stringWithFormat:NSLocalizedString(@"Password should allow up to 50 characters only", @"Password should allow up to 50 characters only"), tagValue]];
        return;
    }
    
    BOOL status = [PasswordManager addPassword:passwordValue withTag:tagValue forWebsite:websiteURL];
    
    if (status) {
        [self.navigationController popViewControllerAnimated:YES];
    } else {
        [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!") andMessage:[NSString stringWithFormat:NSLocalizedString(@"Label name \"%@\" already exists", @"Failed to add Password"), tagValue]];
    }
}

@end