//
//  CustomTextField.m
//  PasswordVault
//
//  Created by Satyanarayana on 01/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "CustomTextField.h"
#import "Utility.h"

@implementation CustomTextField

-(id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        self.layer.borderColor = (__bridge CGColorRef)([Utility colorFromHexString:DARK_TEAL]);
        self.layer.cornerRadius = 3;
        self.layer.borderWidth = 2;
        
        UIView* paddingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 15, 20)];
        self.leftView = paddingView;
        self.leftViewMode = UITextFieldViewModeAlways;
    }
    
    return self;
}

@end
