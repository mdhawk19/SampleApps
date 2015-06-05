//
//  PMAlertView.h
//  PasswordVault
//
//  Created by Satyanarayana SVV on 1/8/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PMTextField.h"


/**
 \brief
 PMAlertView class acts as a component to create different types of alerts which are used in the application. 
 
 This class helps to handle the actions related to all different types of alerts being presented in this application.
 
 This component handles below functions related alerts:
 a) Presenting different types of alerts.
 b) Handling actions specific to the presented alert type like change password will have user entered data to be sent to the repository.
 c) Giving confirmation the completed tasks like password copied to the clipboad and password deleted.

 */

/**
 \enum enum PMAlertType: NSInteger
 Enum to specify the type of the alert view.
 */
typedef NS_ENUM(NSInteger, PMAlertType) {
    PMAlertTypeChangePassword = 0,/**< Custom Alert view for change password action. */
    PMAlertTypePasswordChanged = 1,/**< Custom Alert view for changed password confirmation. */
    PMAlertTypeRemovePassword = 2,/**< Custom Alert view for remove password action.  */
    PMAlertTypePasswordRemoved = 3,/**< Custom Alertview for password removed confirmation. */
    PMAlertTypeRemoveAllPassword = 4,/**< Custom Alertview for remove all password action. */
    PMAlertTypeAllPasswordRemoved = 5,/**<Custom Alert view for all password removed confirmation. */
    PMAlertTypeUnauthorizeNymi = 6,/**< Custom ALertview for Unauthorize Nymi action. */
    PMAlertTypeNymiUnauthorized = 7,/**< Custom alertview for Confirmation of Unauthorize action. */
    PMAlertTypeCopyPassword = 8,/**<  Custom alertView for copy password action.*/
    PMAlertTypePasswordCopied = 9,/**< Custom alertview for confirmation of copy password action. */
    PMAlertTypeViewPassword = 10,/**< Custom alertview for View Password action. */
    PMAlertTypeGeneralError = 11,/**< Custom alertview to display genic errors in the application. */
};


/** Blocks
 
 Blocks are a language-level feature added to Objective-C which allow you to create distinct segments of code that can be passed around to methods or functions as if they were values. Blocks are Objective-C objects, which means they can be added to collections like NSArray or NSDictionary. They also have the ability to capture values from the enclosing scope, making them similar to closures or lambdas in other programming languages.
Below are the blockes used in this application to manage event switch between alertviews.
 */
/**
 \typedef typedef void(^PMAlertCloseBlock)(void)
 
 This is the definition of "block" that will be used to close the Alertview from the Alertviewcontroller .
 
 the class which creates the alertview will have to implement this block to handle proper closure of the same.
 
 */
typedef void(^PMAlertCloseBlock)(void);


/**
 \typedef typedef void(^PMAlertCopyPasswordBlock)(NSString *)
 
 This is the definition of "block" that will handle the coping of the password to the clipboard and gives the confirmation message to the user.
 
 @param NSString the password which has to be copied to the clipboard.
 
 */
typedef void(^PMAlertCopyPasswordBlock)(NSString *);


/**
 \typedef typedef void(^PMAlertPasswordSaveBlock)(NSString * , NSString *, NSString *)
 
 This is the definition of "block" that will handle the change password action. In the change password alertview when the user clicks save button after entering the new password and url this block will get called. 
 
 This blocks gets the Label of the password, new password and changed url of the password to complete the action.
 
 @param NSString the label of the password which requires the update.
 @param NSString the new password.
 @param NSString the new url if changed or the old url.
 
 */
typedef void(^PMAlertPasswordSaveBlock)(NSString * , NSString *, NSString *);


/**
 \typedef typedef void(^PMAlertRemovePasswordBlock)(NSString *)
 
 This is the definition of "block" that will handle the remove password action and once the user confirms to remove the password this block will delete the password from the repository and gives the confirmation message to the user.
 @param NSString the Label of the password whose details has to be be deleted.
 
 */
typedef void(^PMAlertRemovePasswordBlock)(NSString *);

/**
 \typedef typedef void(^PMAlertRemoveAllPasswordBlock)()
 
 This is the definition of "block" that will handle remove all passwords action and once the user confirms to remove all passwords this block will delete all the passwords from the repository and gives the confirmation message to the user.
 */
typedef void(^PMAlertRemoveAllPasswordBlock)();


/**
 \typedef typedef void(^PMAlertUnauthorizeNymiBlock)()
 
 This is the definition of "block" that will handle UnauthorizeNymi action and once the user confirms to Unauthorize, this block will delete all the data related to the user and the user will be taken back to the setup page.
 */
typedef void(^PMAlertUnauthorizeNymiBlock)();


@interface PMAlertView : UIView

/**
 This static method which acts as a constructor to create the type of the alertview required by the user.
 
 @param iAlertType the type of the alertview which needs to be instantiated.
 @return instance of the PMAlertView class.
 */
+ (PMAlertView *)getAlertViewWithType:(PMAlertType)iAlertType;

/**
 \property PMAlertCloseBlock closeAlertBlock
  The alertview which is show has to be closed by accepting user action. the user action will be captured by AlertViewController. this property allows AlertViewController to handle proper closer of the alertview being displayed.
 */
@property(nonatomic,strong) PMAlertCloseBlock closeAlertBlock;

/**
 \property PMAlertCopyPasswordBlock passwordCopyAlertBlock
 The CopypasswordAlertview which is show has to copy the password to the clipboard on click of copy button. the user action will be captured by AlertViewController. this property allows AlertViewController to handle the action.
 */
@property(nonatomic,strong) PMAlertCopyPasswordBlock passwordCopyAlertBlock;

/**
 \property PMAlertPasswordSaveBlock passwaordSaveAlertBlock
 The save password action from change password alertview has to update the new details to teh repository. This property allows AlertViewController to set the action method to complete the task.
 */
@property(nonatomic,strong) PMAlertPasswordSaveBlock passwaordSaveAlertBlock;

/**
 \property PMAlertRemovePasswordBlock removePasswaordAlertBlock
 On user confirmation to remove the selected paasowd, AlertViewController will set the action method using this property.
 */
@property(nonatomic,strong) PMAlertRemovePasswordBlock removePasswaordAlertBlock;

/**
 \property PMAlertRemoveAllPasswordBlock removeAllPasswaordAlertBlock
  On user confirmation to remove all paasowds, AlertViewController will set the action method using this property.
 */
@property(nonatomic,strong) PMAlertRemoveAllPasswordBlock removeAllPasswaordAlertBlock;

/**
 \property PMAlertUnauthorizeNymiBlock unauthorizeNymiAlertBlock
 The alertview which takes the confirmation from the user to Unauthorize the user will have to handle deletion of all data related to that user. this property allows to handle the action after completion of teh task.
 */
@property(nonatomic,strong) PMAlertUnauthorizeNymiBlock unauthorizeNymiAlertBlock;

/**
 \property PMTextField passwordTextFiled
 
 The property which acts as a outlet to password textfield in the storyboard.
 */
@property (weak,nonatomic) IBOutlet PMTextField *passwordTextFiled;

/**
 \property PMTextField websiteTextField
 
 The property which acts as a outlet to website url textfield in the storyboard.
 */
@property (weak, nonatomic) IBOutlet PMTextField *websiteTextField;


/**
 This method will allow to update the title of the alert.
 
 This method acts as a setter for title of alertview depending on the selected row in the passowrd list.
 @param NSString *iTitle title which needs to be displayed on the alertview.
 */
- (void)updateTitle:(NSString *)iTitle;

/**
 This method will set the value of the password in ViewPassword and Copy to clipboard alert types.
 
 While displaying these two alert types, the corresponding password of the label needs to be fetched from the repository and the decrypted password will be shown to the user.
 @param NSString *iPassword decrypted password of the label.
 */
- (void)updatePasswordLabel:(NSString *)iPassword;

/**
 This method will set the value of website url in change password alerttype.
 
 While changing the password, user has the previlage to change websiteurl as well.using this method we can show already saved value of the website url to the user.
 @param NSString *websiteurl website url value.
 */
- (void)updateWebsiteURL:(NSString*)websiteurl;

@end


