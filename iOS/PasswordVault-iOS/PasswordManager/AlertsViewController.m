//
//  AlertsViewController.m
//  PasswordVault
//
//  Created by Satyanarayana on 01/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "AlertsViewController.h"
#import "CustomTextField.h"
#import "Constants.h"
#import "PasswordManager.h"

#import "NSString+Base64.h"
#import "AESCrypt.h"
#import "OptionsViewController.h"

@interface AlertsViewController ()<UITextFieldDelegate>

@property (strong, nonatomic) PMAlertView *alert;

@end

@implementation AlertsViewController

-(void)viewDidLoad {
    [super viewDidLoad];
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] init];
    [tapGesture addTarget:self action:@selector(closeAction)];
    [self.view addGestureRecognizer:tapGesture];
}

/**
 This method is part of View's life cycle. As soon as Controller's view is added to view hierarcy, controller will be notified.
 
 You can over ride this method to handle any of the UI customization or any other task associated with it. You must call "super" at some point of implementation.
 */
-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.view.backgroundColor = [UIColor colorWithPatternImage:TRANSPARENT_IMAGE];    
    [self placeAlert];
}

/**
 
 Depending on the user selection from the OptionViewCOntroller the Alertview sets the type of alertneeds to be created. using this imformation, this method creates the AlertView instance and populates with specific data fields in it along with actions mapped to it.
 */
- (void)placeAlert
{    
    self.alert = nil;
    CGPoint ctr = self.view.center;
    
     switch (self.alertViewType) {
         case PMAlertTypeViewPassword:
         {
             self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeViewPassword];
             
             NSString *password = [[[PasswordManager sharedInstance] selectedPassword] valueForKey:PWD];
             [self.alert updatePasswordLabel:[AESCrypt decrypt:password]];
             [self.alert updateTitle:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:TAG]];
             break;
         }
             
         case PMAlertTypeCopyPassword:
         {
             self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeCopyPassword];
             
             NSString *password = [[[PasswordManager sharedInstance] selectedPassword] valueForKey:PWD];
            [self.alert updatePasswordLabel:[AESCrypt decrypt:password]];
            [self.alert updateTitle:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:TAG]];
             break;
         }
             
        case PMAlertTypeChangePassword:
         {
             self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeChangePassword];
              [self.alert updateTitle:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:TAG]];
             [self.alert updateWebsiteURL:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:WEBSITE_URL]];
             self.alert.passwordTextFiled.delegate = self;
             ctr.y -= 80;
             break;
         }
             
         case PMAlertTypePasswordChanged:
         {
             self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypePasswordChanged];
              [self.alert updateTitle:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:TAG]];
             break;

         }
         case PMAlertTypeRemovePassword:
         {
             self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeRemovePassword];
              [self.alert updateTitle:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:TAG]];
             break;
         }
         case PMAlertTypeRemoveAllPassword:
         {
             self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeRemoveAllPassword];
             break;
         }
         case PMAlertTypeUnauthorizeNymi:
         {
             self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeUnauthorizeNymi];
             break;
         }

             default:
             break;
     }
    
    __weak  AlertsViewController * selfForBlock = self ;
    
    self.alert.closeAlertBlock = ^{
        [selfForBlock callCloseAction];
    };
    
    self.alert.passwaordSaveAlertBlock = ^(NSString *newPassword , NSString *selectedTag, NSString* websiteURL){
        [PasswordManager editPassword:newPassword withTag:selectedTag forWebsite:websiteURL];
        [selfForBlock callPasswordCahngedPopUp];
    };
    
    self.alert.passwordCopyAlertBlock = ^(NSString *password){
        UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
        [pasteboard setString:password];
        [selfForBlock callPasswordCopiedPopUp];
    };
    
    self.alert.removePasswaordAlertBlock  = ^(NSString *tag){
        [PasswordManager removeTag:tag];
        [selfForBlock callPasswordRemovedPopUp];
    };

    self.alert.removeAllPasswaordAlertBlock = ^{
        [PasswordManager purgeAllTags];
//        [selfForBlock.delegate didClearAllPasswords];
        [selfForBlock callAllPasswordRemovedPopUp];
    };
    self.alert.unauthorizeNymiAlertBlock = ^{
        [selfForBlock callUnAutgorizedPopUp];
    };
    
    [self.view addSubview:self.alert];
    [self.alert setCenter:ctr];
}

/**
 This method acts as a completionhandler code to the Alertview controller which helps the alertview to be closed.
 This is a generic method which allows all type of alerts to be closed.
 */
- (void)callCloseAction {
    [self.alert removeFromSuperview];
    if (self.alertViewType == PMAlertTypeNymiUnauthorized) {
        [self.delegate didUserUnAuthorized];
    }
    else if ([[self.navigationController.viewControllers objectAtIndex:0] isKindOfClass:[OptionsViewController class]]) {
        OptionsViewController *optionsViewController = (OptionsViewController *)[self.navigationController.viewControllers objectAtIndex:0];
        [optionsViewController closeButtonTouched:nil];
        [self.navigationController popViewControllerAnimated:NO];
    } else{
        [self.delegate cancelPopUP];
    }
}

/**
 This method acts as a completionhandler code to the AlertViewController of type ChangePassword. once the user enters the new password and taps save button this method will get called to show the confirmation popup.
 */
- (void)callPasswordCahngedPopUp{
    
    [self.alert removeFromSuperview];
    CGPoint ctr = self.view.center;
    self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypePasswordChanged];
    
    [self.alert updateTitle:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:TAG]];
    
    ctr.y -= 80;
    __weak  AlertsViewController * selfForBlock = self ;
    
    self.alert.closeAlertBlock = ^{
        [selfForBlock callCloseAction];
    };
    
    [self.view addSubview:self.alert];
    [self.alert setCenter:ctr];
}

/**
 This method acts as a completionhandler code to the AlertViewController of type CopyPassword. whne user taps copy button from the copyPassword popup, this method will get called and shows confirmation popup to the user.
 */
- (void)callPasswordCopiedPopUp {
    
    [self.alert removeFromSuperview];
    CGPoint ctr = self.view.center;
    self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypePasswordCopied];
    
    PasswordManager *passwordManager = [PasswordManager sharedInstance];
    [self.alert updateTitle:[passwordManager.selectedPassword valueForKey:TAG]];
    
    __weak  AlertsViewController * selfForBlock = self ;
    self.alert.closeAlertBlock = ^{
        [selfForBlock callCloseAction];
    };
    
    
    [self.view addSubview:self.alert];
    [self.alert setCenter:ctr];
}

/**
 This method acts as a completionhandler code to the AlertViewController of type RemovePassword, which confirms the action of removing password.
 */
- (void)callPasswordRemovedPopUp{
    [self.alert removeFromSuperview];
    CGPoint ctr = self.view.center;
    self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypePasswordRemoved];
    [self.alert updateTitle:[[[PasswordManager sharedInstance] selectedPassword] valueForKey:TAG]];
    
    __weak  AlertsViewController * selfForBlock = self ;
    
    self.alert.closeAlertBlock = ^{
        [selfForBlock callCloseAction];
    };
    
    [self.view addSubview:self.alert];
    [self.alert setCenter:ctr];
}


/**
 This method acts as a completionhandler code to the AlertViewController of type ClerAllPasswords, which confirms the action of clearing all passwords.
 */
- (void)callAllPasswordRemovedPopUp {
    
    [self.alert removeFromSuperview];
    CGPoint ctr = self.view.center;
    ctr.x -= 10;
    
    self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeAllPasswordRemoved];
    __weak  AlertsViewController * selfForBlock = self ;
    
    self.alert.closeAlertBlock = ^{
        [selfForBlock callCloseAction];
    };
    
    [self.view addSubview:self.alert];
    [self.alert setCenter:ctr];
}

/**
 This method acts as a completionhandler code to the AlertViewController of type UnAuthorize User, which takes care of unauthorizing the user.
 */
- (void)callUnAutgorizedPopUp {
    
    [self.alert removeFromSuperview];
    CGPoint ctr = self.view.center;
    ctr.x -= 10;
    self.alert =  [PMAlertView getAlertViewWithType:PMAlertTypeNymiUnauthorized];
    self.alertViewType = PMAlertTypeNymiUnauthorized;
    __weak  AlertsViewController * selfForBlock = self ;
    
    self.alert.closeAlertBlock = ^{
        [selfForBlock closeAction];
    };
    
    
    [self.view addSubview:self.alert];
    [self.alert setCenter:ctr];
    [[NSUserDefaults standardUserDefaults] setValue:@"" forKey:PROV_ID];
    [PasswordManager purgeAllTags];
}


/**
 This method is a delegate of UITextFieldDelegate which tells the implemeting class that the user has ended editing.*/
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField{
    [textField resignFirstResponder];
    return YES;
}

/**
 This method is a delegate to UITextFieldDelegate which tells the implementing class that the user has tapped on return button on the keyboard.
 */
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

/**
 This method is a delegate to UITextFieldDelegate which tells the implementing class the user has entered new values in the textfield.
 */
- (BOOL)textField:(UITextField *) textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string {
    NSUInteger oldLength = [textField.text length];
    NSUInteger replacementLength = [string length];
    NSUInteger rangeLength = range.length;
    NSUInteger newLength = oldLength - rangeLength + replacementLength;
    BOOL returnKey = [string rangeOfString: @"\n"].location != NSNotFound;
    return newLength <= MAXLENGTH_PASSWORD || returnKey;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation NS_DEPRECATED_IOS(2_0,8_0) {
    [super didRotateFromInterfaceOrientation: fromInterfaceOrientation];
    CGPoint ctr = self.view.center;
    [self.alert setCenter:ctr];
}


/**
 This method allows the user to close the popup by clicking outside of the popup using the tap guesture.
 */
- (void)closeAction {
    [self callCloseAction];
}

@end
