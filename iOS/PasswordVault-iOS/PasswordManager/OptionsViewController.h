//
//  OptionsViewController.h
//  PasswordVault
//
//  Created by Satyanarayana on 01/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>


 /**
 @protocol OptionsDelegate "PasswordManager/OptionsViewController"
 \brief
 A protocol defines the methods expected to be used for a particular situation.
 
The actions like Closing of optionview and Launching URl needs to be handled by MainViewController.this delegate helps in giving the control back to the MainViewController.
 */

@protocol OptionsDelegate <NSObject>
/**
 This method allows the closing of the popup view. The implementing class can perform action like updation of the list after closing the popup.
 */
-(void)hideOptions;

/**
 This method will get called once user selects Luanch URL from the OptionsViewController. The implementing class can check the validity of the URL and continue with the action.
 */
-(void)openLabelURL;

@end


/**
 \brief The OptionsViewController class  is responsible for creating the option view.
 
 This ViewController mappes the user action like ViewPassword, Copy Password, Change Password, Remove Password, Launch URL to specific popuptypes with the help of different segues.
 */

@interface OptionsViewController : UIViewController

@property (strong, nonatomic) IBOutlet UILabel *tagLabel;
@property(nonatomic, assign) id<OptionsDelegate> delegate;
@property (weak, nonatomic) IBOutlet UIButton *removeButton;

- (IBAction)closeButtonTouched:(id)isender;
@end
