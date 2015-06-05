//
//  NCLWrapper.h
//  BioNym
//
//  Created by Satyanarayana SVV on 11/21/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "AESCrypt.h"

#import "NSData+Base64.h"
#import "NSString+Base64.h"
#import "NSData+CommonCrypto.h"

#pragma mark - Encyrption Key

#define ENCRYPT_PASSWORD @"24A008E9294C238BC5B2DE0907C224887427820784BEBCC695897E9E29B4D0AF"


@implementation AESCrypt

+ (NSString *)encrypt:(NSString *)message {
  NSData *encryptedData = [[message dataUsingEncoding:NSUTF8StringEncoding] AES256EncryptedDataUsingKey:[[ENCRYPT_PASSWORD dataUsingEncoding:NSUTF8StringEncoding] SHA256Hash] error:nil];
  NSString *base64EncodedString = [NSString base64StringFromData:encryptedData length:[encryptedData length]];
  return base64EncodedString;
}

+ (NSString *)decrypt:(NSString *)base64EncodedString {
  NSData *encryptedData = [NSData base64DataFromString:base64EncodedString];
  NSData *decryptedData = [encryptedData decryptedAES256DataUsingKey:[[ENCRYPT_PASSWORD dataUsingEncoding:NSUTF8StringEncoding] SHA256Hash] error:nil];
  return [[NSString alloc] initWithData:decryptedData encoding:NSUTF8StringEncoding];
}

@end
