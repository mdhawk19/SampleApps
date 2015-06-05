//
//  NCLWrapper.h
//  BioNym
//
//  Created by Satyanarayana SVV on 11/21/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

@import Foundation;

@interface NSString (Base64Additions)

+ (NSString *)base64StringFromData:(NSData *)data length:(NSUInteger)length;

@end
