//
//  ShowNoAnimationSegue.m
//  PasswordVault
//
//  Created by Satyanarayana on 22/01/15.
//  Copyright (c) 2015 HappiestMinds. All rights reserved.
//

#import "ShowNoAnimationSegue.h"

@implementation ShowNoAnimationSegue

-(void) perform{
    [[[self sourceViewController] navigationController] pushViewController:[self   destinationViewController] animated:NO];
}

@end
