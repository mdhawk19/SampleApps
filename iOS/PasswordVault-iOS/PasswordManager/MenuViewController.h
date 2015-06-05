//
//  MenuViewController.h
//  BioNym
//
//  Created by Satyanarayana SVV on 12/18/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

/**
 @protocol MenuDelegate "PasswordManager/MenuViewController.h"
 \brief 
 A protocol defines the methods expected to be used for a particular situation.
 
 Actions executed in Menu needs to be propogated to other view controllers. This is achieved using "MenuDelegate".
 
 All the methods are mandatory to be implemented in view controller that acts as delegate for Menu
 */
@protocol MenuDelegate <NSObject>

/**
 Asks the delegate to capture Label, Password and URL(optional) as a new entry to be created in the list.
 */
-(void)addNewEntry;

/**
 Asks the delegate to filter the Labels based on user selection "All" or "Starred".
 
 This method fetches all the passwords from the backend if the showAll parameter is set to YES.
 @param showAll  If YES then all the Lables which are stored by the are fetched and displayed. If its NO, only Labels marked as "Star" will be displayed.
 */
-(void)showEntries:(BOOL)showAll;


/**
 Asks the delegate to clear all the Labels/Passwords stored in the local storage.
 */
-(void)clearAllPasswords;

/**
 Asks the delegate the clear all the Labels/Passwords and Unauthorize the Nymi band.
 */
-(void)unAuthorize;

@end

/**
 \brief
 This screen represents the Menu of actions to be performed on the "Password Vault" functionality.
 Actions in this view includes Add New Label, Filter the Labels based on selection "All" or "Starred", Option to clear all Labels and Option to unauthorize the Nymi band
 */
@interface MenuViewController : UIViewController

/**
 \property delegate
 The object that acts and receives the calls for handling Menu.
 */
@property(nonatomic, weak) id<MenuDelegate> delegate;

@end
