//
//  NCLWrapper.h
//  BioNym
//
//  Created by Satyanarayana SVV on 11/21/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

@import Foundation;

@interface AESCrypt : NSObject

+ (NSString *)encrypt:(NSString *)message;
+ (NSString *)decrypt:(NSString *)base64EncodedString;

@end
