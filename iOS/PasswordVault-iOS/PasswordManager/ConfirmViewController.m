//
//  ConfirmViewController.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "ConfirmViewController.h"
#import "NCLWrapper.h"
#import "AddPasswordViewController.h"
#import "Utility.h"

@interface ConfirmViewController ()

@property (weak, nonatomic) IBOutlet UIView *buttonsView;
@property (weak, nonatomic) IBOutlet UIView *authorizingView;
@property (weak, nonatomic) IBOutlet UIButton *acceptButton;
@property (weak, nonatomic) IBOutlet UIButton *rejectButton;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *actIndicator;
@property (weak, nonatomic) IBOutlet UILabel *authorizingLbl;
@property (weak, nonatomic) IBOutlet UIButton *authorized;
@property (weak, nonatomic) IBOutlet UIImageView *firstLED;
@property (weak, nonatomic) IBOutlet UIImageView *secondLED;
@property (weak, nonatomic) IBOutlet UIImageView *thirdLED;
@property (weak, nonatomic) IBOutlet UIImageView *fourthLED;
@property (weak, nonatomic) IBOutlet UIImageView *fifthLED;

@end

@implementation ConfirmViewController
@synthesize agreeCode;

/**
 This method is part of view's life cycle. It will be called as soon as controller's view is loaded into memory. This method will be called regardless of wheter view is loaded from XIB or storyboard or created programatically. You usually override this method to perform additional initialization on views that were loaded from nib files.
 
 Customize LEDs based on agree code received and UI accordingly
 */
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    if ([self.agreeCode characterAtIndex:0] == '1') {
        self.firstLED.highlighted = YES;
    }
    if ([self.agreeCode characterAtIndex:1] == '1') {
        self.secondLED.highlighted = YES;
    }
    if ([self.agreeCode characterAtIndex:2] == '1') {
        self.thirdLED.highlighted = YES;
    }
    if ([self.agreeCode characterAtIndex:3] == '1') {
        self.fourthLED.highlighted = YES;
    }
    if ([self.agreeCode characterAtIndex:4] == '1') {
        self.fifthLED.highlighted = YES;
    }

    self.acceptButton.layer.cornerRadius = 4;
    self.rejectButton.layer.cornerRadius = 4;
    
    self.acceptButton.layer.borderWidth = 1;
    self.rejectButton.layer.borderWidth = 1;
    
    [self.acceptButton setTitleColor:[UIColor orangeColor] forState:UIControlStateNormal];
    [self.rejectButton setTitleColor:[UIColor lightGrayColor] forState:UIControlStateNormal];
    
    self.acceptButton.layer.borderColor = [UIColor orangeColor].CGColor;
    self.rejectButton.layer.borderColor = [UIColor lightGrayColor].CGColor;
    
}

/**
 When the app receives memory warning, this notification will be sent to view controller. You should never call this method directly. When the system determines that the app is occupying more memory than allocated, it will throw a notification "available memory is low"
 
 In this method you can release any of the objects that are taking more memory like arrays etc.
 */
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


/**
 This is the action that will be performed when user taps on the button "YES, THAT'S MY NYMI BAND"
 This method will start provisioning Nymi band that was discovered earlier.
 UI controls are updated accordingly.
 Upon succesfull provisioning, user will be taken to validation screen. Else, it will show failure buton. On tapping that, user will be taken back to "Set Up" screen to start provisioning again. 
 @param sender sender object, here the button that is tapped
 */
- (IBAction)acceptNymi:(id)sender {    
    self.buttonsView.hidden = YES;
    self.authorizingView.hidden = NO;
    [[NCLWrapper sharedInstance] provisionNymi:^(NSDictionary* params, NSError *err) {
        dispatch_async(dispatch_get_main_queue(), ^{
            self.actIndicator.hidden = YES;
            self.authorizingLbl.hidden = YES;
            self.authorized.hidden = NO;
            if (err) {
                [self.authorized setImage:FAILURE_IMAGE forState:UIControlStateNormal];
            } else {
                [self.authorized setImage:SUCCESS_IMAGE forState:UIControlStateNormal];
                [self performSelector:@selector(callValidateController) withObject:nil afterDelay:1];
            }
        });
    }];
}


/**
 This is the action that will be executed on tapping the button "NO, I DON'T SEE THAT".
 User can reject if the LEDs signature is not matching by tapping above said button. It will take the user to beginning screen to start "Set up" again.
 @param sender sender object, here the button that is tapped
 */
- (IBAction)rejectNymi:(id)sender {
    [self.navigationController popToRootViewControllerAnimated:YES];
}

/**
 Method that will take perorm "Validate" view controller segue.
 */
- (void)callValidateController {
    [self performSegueWithIdentifier:VALIDATE_SEGUE sender:nil];
}


@end
