//
//  PMTextField.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 1/8/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "PMTextField.h"
#import "Utility.h"

@implementation PMTextField

/**
 This custroctor method helps to set the common properties of the Textfield.
 */
-(id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        self.layer.borderColor = [UIColor darkGrayColor].CGColor;
        self.layer.borderWidth = 1;
        self.layer.cornerRadius = 3;
        
        UIView* paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 20, 30)];
        self.leftView = paddingView;
        self.leftViewMode = UITextFieldViewModeAlways;
    }
    
    return self;
}

@end
