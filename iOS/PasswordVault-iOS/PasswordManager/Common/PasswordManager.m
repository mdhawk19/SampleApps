//
//  PasswordVault.m
//  BioNym
//
//  Created by Satyanarayana SVV on 12/17/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "PasswordManager.h"
#import "AESCrypt.h"
#import "Constants.h"

#define ENTRIES_FILE @"Entries.plist"

@implementation PasswordManager

/**
 This static method helps to access this class as Sigleton instance.
 @return singleton instance of the PasswordManager class.
 */
+(id)sharedInstance {
    static dispatch_once_t onceToken;
    static PasswordManager* passwordManager = nil;
    dispatch_once(&onceToken, ^{
        passwordManager = [[self alloc] init];
    });
    
    return passwordManager;
}

/**
 Fetches all the list of Labels and associated values from plist file.
 
 @return NSArray Array of Labels
 */
+(NSArray*)getAllTags {
    return [NSArray arrayWithContentsOfFile:[self getPasswordFilePath]];
}

/**
 Adds a Label, Password and web site URL (if available) to the plist. First it checks if an entry with same Label is available or not. If its not found it will be added. If its already it return NO.
 By default it will store the entry as Not starred.
 
 @param password Password
 @param tag Label
 @param website URL of the website with which password and Label are associated.
 @return bool Returns YES if its successfully added, else NO.
 
 */
+(BOOL)addPassword:(NSString*)password withTag:(NSString*)tag forWebsite:(NSString*)website {
    BOOL status = NO;
    NSMutableArray* tags = [NSMutableArray arrayWithContentsOfFile:[self getPasswordFilePath]];
    
    //Search for item if an entry with same tag name is already added.
    NSInteger indx = [tags indexOfObjectPassingTest:^BOOL(id obj, NSUInteger idx, BOOL *stop) {
        return [obj[TAG] isEqualToString:tag];
    }];
    
    if (indx == NSNotFound) {
        [tags addObject:@{
                             TAG:tag,
                             PWD: [AESCrypt encrypt:password],
                             WEBSITE_URL : website ? website : @"",
                             STAR: @NO
                            }];
        [tags writeToFile:[self getPasswordFilePath] atomically:YES];
        status = YES;
    }
    
    return status;
}

/**
 Edit the label and its associated details. It will update the plist file with necessary changes.
 
 @param password Password
 @param tag Label
 @param website URL of the website with which password and Label are associated.
 @return bool Returns YES if its successfully edited, else NO.
 */
+(BOOL)editPassword:(NSString*)password withTag:(NSString*)tag forWebsite:(NSString*)website {
    BOOL status = NO;
    NSMutableArray* tags = [NSMutableArray arrayWithContentsOfFile:[self getPasswordFilePath]];
    
    //Search for item if an entry with same tag name is present
    NSInteger indx = [tags indexOfObjectPassingTest:^BOOL(id obj, NSUInteger idx, BOOL *stop) {
        return [obj[TAG] isEqualToString:tag];
    }];
    
    if (indx != NSNotFound) {
        NSDictionary* tagData = tags[indx];
        NSDictionary *newTag = @{
                                 TAG : tag,
                                 PWD : [AESCrypt encrypt:password],
                                 WEBSITE_URL : website ? website : @"",
                                 STAR : tagData[STAR]
                                 };
        [tags replaceObjectAtIndex:indx withObject:newTag];
        [[PasswordManager sharedInstance] setSelectedPassword:newTag];
        [tags writeToFile:[self getPasswordFilePath] atomically:YES];
        status = YES;
    }
    
    return status;
}

/**
 Removes the Label and associated details from plist. It will query the plist file and removes it according to its availability.
 
 @param tag Label
 @return bool Returns YES if its successfully removed, else NO.
 */
+(BOOL)removeTag:(NSString*)tag {
    BOOL status = NO;
    NSMutableArray* tags = [NSMutableArray arrayWithContentsOfFile:[self getPasswordFilePath]];
    
    //Search for item if an entry with same tag name is present
    NSInteger indx = [tags indexOfObjectPassingTest:^BOOL(id obj, NSUInteger idx, BOOL *stop) {
        return [obj[TAG] isEqualToString:tag];
    }];
    
    if (indx != NSNotFound) {
        [tags removeObjectAtIndex:indx];
        [tags writeToFile:[self getPasswordFilePath] atomically:YES];
        status = YES;
    }

    return status;
}


/**
 Clears all the Labels and associated informtaion from plist.
 */
+(void)purgeAllTags {
    NSMutableArray* entries = [NSMutableArray arrayWithContentsOfFile:[self getPasswordFilePath]];
    [entries removeAllObjects];
    [entries writeToFile:[self getPasswordFilePath] atomically:YES];
}

/**
 Prepares the Password file path and returns it. Its helper function to access plist file.
 @return NSString Path for plist file
 */
+(NSString*)getPasswordFilePath {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString* pwdFile = [documentsDirectory stringByAppendingPathComponent:ENTRIES_FILE];
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:pwdFile]) {
        NSMutableArray* temp = [@[] mutableCopy];
        [temp writeToFile:pwdFile atomically:YES];
    }
    
    return pwdFile;
}

/**
 Star on Unstar a Label.
 
 @param tag Label to Star or Unstar.
 @param starUnStar If its YES, Label will be set as "Star", NO to set as "UnStar"
 @return bool Returns YES if its successfully Star/Unstar, else NO.
 */
+(BOOL)starUnStartPasswordwithTag:(NSString*)tag {
    BOOL status = NO;
    NSMutableArray* tags = [NSMutableArray arrayWithContentsOfFile:[self getPasswordFilePath]];
    
    //Search for item if an entry with same tag name is present
    NSInteger indx = [tags indexOfObjectPassingTest:^BOOL(id obj, NSUInteger idx, BOOL *stop) {
        return [obj[TAG] isEqualToString:tag];
    }];
    
    if (indx != NSNotFound) {
        NSDictionary* tagData = tags[indx];
        
        [tags replaceObjectAtIndex:indx withObject:@{
                                                     TAG : tag,
                                                     PWD : tagData[PWD],
                                                     WEBSITE_URL : tagData[WEBSITE_URL],
                                                     STAR :([tagData[STAR] boolValue] ? @NO : @YES)
                                                     }];
        [tags writeToFile:[self getPasswordFilePath] atomically:YES];
        status = YES;
    }
    
    return status;
}

@end
