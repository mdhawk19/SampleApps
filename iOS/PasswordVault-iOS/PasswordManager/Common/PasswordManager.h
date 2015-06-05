//
//  PasswordVault.h
//  BioNym
//
//  Created by Satyanarayana SVV on 12/17/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 \brief 
 This is a helper class to handle the Labels for permanent storage.
 
 All the Labels, Passwords etc are stored in a simple encrypted plist in side NSDcoumentsDirectory of the application. Its very secured one as no one can access NSDocumentsDirectory from outside the application and the other secured featured is to encrypt the passwords with 128 bit key. AES encryption algorithm with 128 bit key is used for encrption.
 
 This class provides methods to get all the list of Labels, add new Label, edit a Label and associated values, remove a particular Label, clear all Labels, Star or UnStart a Label etc.
 
 This is a singleton object and so can be accessed from any where in the application to access the details.
 */
@interface PasswordManager : NSObject

/**
 Selected object in the list for accessing by various operations like Edit etc.
 */
@property (strong, nonatomic) NSDictionary *selectedPassword;

/**
  This static method helps to access this class as Sigalton instance.
 @return singalton instance of the PasswordManager class.
 */
+(id)sharedInstance;

/**
 Fetches all the list of Labels and associated values from plist file.
 
 @return NSArray Array of Labels
 */
+(NSArray*)getAllTags;

/**
 Adds a Label, Password and web site URL (if available) to the plist. First it checks if an entry with same Label is available or not. If its not found it will be added. If its already it return NO.
 By default it will store the entry as Not starred.

 @param password Password
 @param tag Label
 @param website URL of the website with which password and Label are associated.
 @return bool Returns YES if its successfully added, else NO.
 
 */
+(BOOL)addPassword:(NSString*)password withTag:(NSString*)tag forWebsite:(NSString*)website;

/**
 Edit the label and its associated details. It will update the plist file with necessary changes.

 @param password Password
 @param tag Label
 @param website URL of the website with which password and Label are associated.
 @return bool Returns YES if its successfully edited, else NO.
 */
+(BOOL)editPassword:(NSString*)password withTag:(NSString*)tag forWebsite:(NSString*)website;

/**
 Removes the Label and associated details from plist. It will query the plist file and removes it according to its availability.
 
 @param tag Label
 @return bool Returns YES if its successfully removed, else NO.
 */
+(BOOL)removeTag:(NSString*)tag;

/**
 Clears all the Labels and associated informtaion from plist.
*/
+(void)purgeAllTags;

/**
 Star on Unstar a Label.
 
 @param tag Label to Star or Unstar.
 @param starUnStar If its YES, Label will be set as "Star", NO to set as "UnStar"
 @return bool Returns YES if its successfully Star/Unstar, else NO.
 */
+(BOOL)starUnStartPasswordwithTag:(NSString*)tag;


@end
