//
//  ItemCell.m
//  PasswordVault
//
//  Created by Satyanarayana on 30/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "ItemCell.h"
#import "Utility.h"

@implementation ItemCell

- (void)awakeFromNib
{
    UIView *selectedBackgroundView = [[UIView alloc] initWithFrame:self.bounds];
    selectedBackgroundView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    selectedBackgroundView.backgroundColor = [Utility colorFromHexString:HIGH_LIGHT_COLOR];
    self.selectedBackgroundView = selectedBackgroundView;
}

//- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
//    [super setSelected:selected animated:animated];
//    
//    UIView * selectedBackgroundView = [[UIView alloc] init];
//    [selectedBackgroundView setBackgroundColor:[Utility colorFromHexString:HIGH_LIGHT_COLOR]]; // set color here
//    [self setSelectedBackgroundView:selectedBackgroundView];
//}

/**
 This is the action method which will get called when user taps on the star button of the label listing.
 */
-(IBAction)itemTapped:(id)sender {
    [self.delegate itemTappedAtIndex:self.index];
}

@end
