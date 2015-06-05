//
//  DiscoveryViewController.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 12/22/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "DiscoveryViewController.h"
#import "ConfirmViewController.h"
#import "NCLWrapper.h"
#import "Constants.h"
#import "PaddingLabel.h"

@interface DiscoveryViewController ()

@property (weak, nonatomic) IBOutlet PaddingLabel *topLabel;
@property (weak, nonatomic) IBOutlet UILabel *bottomLabel;
@property (weak, nonatomic) IBOutlet UIButton *retryButton;
@property (weak, nonatomic) IBOutlet UIImageView *animatingSearchImage;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *authorizingIndicator;
@property (strong, nonatomic) NSString *agreeCode;

-(void) startDiscoverNymi;

@end

@implementation DiscoveryViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    self.retryButton.layer.borderWidth = 2;
    self.retryButton.layer.cornerRadius = 3;
    self.retryButton.layer.borderColor = [UIColor lightGrayColor].CGColor;
}


/**
 This method is part of View's life cycle. As soon as Controller's view is added to view hierarcy, controller will be notified.
 
 You can over ride this method to handle any of the UI customization or any other task associated with it. You must call "super" at some point of implementation.
 
 Call NCL Wrapper's "discoverNymiWithHandler" method that will initialize NCL library, upon successful initialization, it will start discovering Nymi band. Once discovery is successful, it will request "agree" code that returns the status of LEDs.
 */
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    self.animatingSearchImage.animationImages =  @[[UIImage imageNamed:@"NEASearch_1"],
                                                 [UIImage imageNamed:@"NEASearch_2"],
                                                 [UIImage imageNamed:@"NEASearch_3"],
                                                 [UIImage imageNamed:@"NEASearch_4"]];
    self.animatingSearchImage.animationDuration = 1.0f;
    self.animatingSearchImage.animationRepeatCount = 0;
    [self.animatingSearchImage startAnimating];
    
    [self startDiscoverNymi];
}


-(void) startDiscoverNymi {
    __weak DiscoveryViewController *weakSelf = self;
    
    [[NCLWrapper sharedInstance] discoverNymi:^(id param, NSError *error) {
        if (error) {
            DLog(@"Error: %@", error.localizedDescription);
            dispatch_async(dispatch_get_main_queue(), ^{
                weakSelf.topLabel.text = NSLocalizedString(@"We can't find your Nymi Band. Try tapping more rapidly and make sure your Nymi Band is activated.", @"We can't find your Nymi Band. Try tapping more rapidly and make sure your Nymi Band is activated.");
                weakSelf.topLabel.layer.cornerRadius = 4;
                weakSelf.topLabel.layer.borderWidth = 1;
                weakSelf.topLabel.layer.borderColor = [UIColor orangeColor].CGColor;
                [weakSelf.authorizingIndicator stopAnimating];
                [weakSelf.animatingSearchImage stopAnimating];
                weakSelf.authorizingIndicator.hidden = YES;
                weakSelf.bottomLabel.hidden = YES;
                weakSelf.retryButton.hidden = NO;
                [weakSelf.topLabel setNeedsUpdateConstraints];
            });
        } else {
            weakSelf.agreeCode = [param[DATA_TAG] substringToIndex:5];
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf performSegueWithIdentifier:AGREEPROVISION_SEGUE sender:nil];
            });
        }
    }];

}


- (IBAction)retryAction:(id)sender {
    self.retryButton.hidden = YES;
    self.topLabel.text = NSLocalizedString(@"Tap your Nymi Band 4 times to enter Pairing Mode. The lights will blink outwards.", @"Tap your Nymi Band 4 times to enter Pairing Mode. The lights will blink outwards.");
    self.topLabel.layer.borderWidth = 0;
    
    self.bottomLabel.hidden = NO;
    self.authorizingIndicator.hidden = NO;
    [self.authorizingIndicator startAnimating];
    [self.animatingSearchImage startAnimating];
    [self startDiscoverNymi];
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
 This method will be used when you use "storyboard" in your application. As and when a new view controller is going to be presented, system will notify the view controller about the new "segue" to be performed.
 
 You have to over ride this method when you have to pass some data to destination view controller that is going to be displayed.
 
 Pass the "agree" code received from NCL library that represents the status of LEDs On/Off. "agree" code is used to match app with device such that the app discovered right Nymi band
 
 @param segue The segue object containing information about the view controllers involved in the segue.
 @param sender The object that initiated the segue. You might use this parameter to perform different actions based on which control (or other object) initiated the segue.
 */
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    
    if ([segue.identifier isEqualToString:AGREEPROVISION_SEGUE]) {
        ConfirmViewController *confirmViewController = (ConfirmViewController *)segue.destinationViewController;
        confirmViewController.agreeCode = self.agreeCode;
    }
}


@end
