    //
//  ValidateViewController.m
//  PasswordVault
//
//  Created by Satyanarayana on 31/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "ValidateViewController.h"
#import "NCLWrapper.h"
#import "Constants.h"
#import "Utility.h"

@interface ValidateViewController ()

@property (weak, nonatomic) IBOutlet UIImageView *validateImage;
@property (weak, nonatomic) IBOutlet UIButton *retryButton;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *activityIndicator;
@property (weak, nonatomic) IBOutlet UILabel *validateLabel;

@end

@implementation ValidateViewController 


/**
 This method is part of view's life cycle. It will be called as soon as controller's view is loaded into memory. This method will be called regardless of wheter view is loaded from XIB or storyboard or created programatically. You usually override this method to perform additional initialization on views that were loaded from nib files.
 
 Customer View's background color
 */
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.view.backgroundColor = [Utility colorFromHexString:DARK_NAVY];
    self.retryButton.backgroundColor = [Utility colorFromHexString:ORANGE_NORMALSATE];
    self.retryButton.layer.cornerRadius = 3;
}

/**
 This method is part of View's life cycle. As soon as Controller's view is added to view hierarcy, controller will be notified.
 
 You can over ride this method to handle any of the UI customization or any other task associated with it. You must call "super" at some point of implementation.
 
 Start validating Nymi band
 */
-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self validateNymi];
}


/**
 Validate Nymi using NCL library. It will first start finding the Nymi band that was provisioned earlier and then start valdating it. Upon successful validation, it will take the user to access Labels listing screen. If failed, UI will be updated accordingly
 */


- (IBAction)retryValidation {
    self.retryButton.hidden = YES;
    self.validateImage.hidden= YES;
    self.activityIndicator.hidden = NO;
    [self.activityIndicator startAnimating];
    [self validateNymi];
}


-(void)validateNymi {
    self.validateLabel.text = NSLocalizedString(@"VERIFYING YOUR NYMI BAND", @"VERIFYING YOUR NYMI BAND");
    [[NCLWrapper sharedInstance] findAndValidateNymi:^(id param, NSError *error) {
        dispatch_async(dispatch_get_main_queue(), ^{
            self.retryButton.hidden = NO;
            [self.activityIndicator stopAnimating];
            self.validateImage.hidden = NO;
            if (error) {
                self.validateImage.hidden = YES;
                self.validateLabel.text = NSLocalizedString(@"COULD NOT VERIFY", @"COULD NOT VERIFY");
            } else {
                self.validateImage.hidden = NO;
                self.retryButton.hidden = YES;
                self.validateImage.image = SUCCESS_IMAGE;
                self.validateLabel.text = NSLocalizedString(@"VERIFIED", @"VERIFIED");
                [self performSelector:@selector(continueAction) withObject:nil afterDelay:1];
            }
        });
    }];
}

/**
 Performs the segue to show "Labels" listing screen
 */
- (void)continueAction {
    if (IS_IPAD) {
        [self performSegueWithIdentifier:CONTAINER_IPAD_SEGUE sender:nil];
    } else {
        [self performSegueWithIdentifier:CONTAINER_SEGUE sender:nil];
    }
}

@end
