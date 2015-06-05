//
//  PaddingLabel.m
//  PasswordVault
//
//  Created by Satyanarayana on 12/02/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "PaddingLabel.h"

@interface PaddingLabel ()

@property (nonatomic) UIEdgeInsets insets;

@end

@implementation PaddingLabel

-(CGSize)intrinsicContentSize{
    CGSize contentSize = [super intrinsicContentSize];
    return CGSizeMake(contentSize.width + 10, contentSize.height + 50);
}

@end
