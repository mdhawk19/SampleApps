//
//  PMButton.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 1/8/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//
#import <QuartzCore/QuartzCore.h>
#import "PMButton.h"

@implementation PMButton
{
    UIColor *_fillColor;
}

/**
 
 This method allows us to set the defualt values after creating the view with the help of xibs.
 */
- (void)awakeFromNib
{
    _fillColor = self.backgroundColor;
    [self setBackgroundColor:[UIColor clearColor]];
}

/**
 This method handles the drawing of border to the button.
 */
-(void)drawRect:(CGRect)rect
{
    CGRect frm = rect;
    frm.origin.x = 1;
    frm.origin.y = 1;
    frm.size.width -= 2;
    frm.size.height -= 2;
    UIBezierPath* path = [UIBezierPath bezierPathWithRoundedRect:frm cornerRadius:3];
    [_fillColor setFill];
    [path fill];
    int thickness = 2;
    path = [UIBezierPath bezierPathWithRoundedRect:CGRectInset(self.bounds, thickness/2,thickness/2) cornerRadius: 3];
    path.lineWidth = thickness;
    [self.titleLabel.textColor setStroke];
    [path stroke];
}


@end
