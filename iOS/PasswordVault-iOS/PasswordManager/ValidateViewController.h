//
//  ValidateViewController.h
//  PasswordVault
//
//  Created by Satyanarayana on 31/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
/**
 \brief 
 Once Nymi band is successfully provisioned after discovery, it has to be validated every time the app will be started unless its unauthorized. This class will handle the validation of Nymi band.
 */
@interface ValidateViewController : UIViewController <UITextFieldDelegate>

@end
