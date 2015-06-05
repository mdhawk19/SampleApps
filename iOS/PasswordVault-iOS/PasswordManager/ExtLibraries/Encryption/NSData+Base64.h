//
//  NCLWrapper.h
//  BioNym
//
//  Created by Satyanarayana SVV on 11/21/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

@import Foundation;

@class NSString;

@interface NSData (Base64Additions)

+ (NSData *)base64DataFromString:(NSString *)string;

@end