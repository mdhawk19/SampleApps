//
//  PMAlertView.m
//  PasswordVault
//
//  Created by Satyanarayana SVV on 1/8/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "PMAlertView.h"
#import "Constants.h"
#import "Utility.h"

@interface PMAlertView ()

@property (weak,nonatomic) IBOutlet UILabel *headerLable;
@property (weak,nonatomic) IBOutlet UILabel *passwordLabel;

@end

@implementation PMAlertView

@synthesize closeAlertBlock =_closeAlertBlock ;
@synthesize passwaordSaveAlertBlock = _passwaordSaveAlertBlock;
@synthesize removePasswaordAlertBlock = _removePasswaordAlertBlock;
@synthesize removeAllPasswaordAlertBlock = _removeAllPasswaordAlertBlock;
@synthesize unauthorizeNymiAlertBlock = _unauthorizeNymiAlertBlock;
@synthesize passwordCopyAlertBlock = _passwordCopyAlertBlock;

/**
 This static method which acts as a constructor to create the type of the alertview required by the user.
 
 @param iAlertType the type of the alertview which needs to be instantiated.
 @return instance of the PMAlertView class.
 */
+ (PMAlertView *)getAlertViewWithType:(PMAlertType)iAlertType {
    NSArray *xibs = [[NSBundle mainBundle] loadNibNamed:ALERTVIEW owner:self options:nil];
    PMAlertView *alertView = (PMAlertView *) [xibs objectAtIndex:iAlertType];
    return alertView;
}


/**
 
 This method allows us to set the defualt values after creating the view with the help of xibs.
 */
- (void)awakeFromNib {
    [self.passwordTextFiled becomeFirstResponder];
}

/**
 This Method Updates the title of the alertview being presented.
 */
- (void)updateTitle:(NSString *)iTitle {
    [self.headerLable setText:iTitle];
}


/**
 This method sets the password of the selected row in the passowrdlist to be shown to the user in  viewpassword and copy to  clipboard type of alertviews.
 */
- (void)updatePasswordLabel:(NSString *)iPassword {
    [self.passwordLabel setText:iPassword];
}

/**
 This method sets the value of the websiteurl textfield in ChangePassword alerttype.
 */
- (void)updateWebsiteURL:(NSString*)websiteurl {
    self.websiteTextField.text = websiteurl;
}


/**
 IBAction method to close the alertview when user taps on close button.
 */
- (IBAction)closeButtonTouched:(id)sender {
    [self hideKeyboard];
    if (self.closeAlertBlock != nil) {
        self.closeAlertBlock();
    }
}

/**
 IBAction Method to copy the password, when user tapes on copy button.
 */
- (IBAction)copyButtonTouched:(id)sender {
    if (_passwordCopyAlertBlock != nil) {
        _passwordCopyAlertBlock(self.passwordLabel.text);
    }
}

/**
 IBACtion Method cancels the user seleted action like cancling of copy to clipboard, deletion etc..
 */
- (IBAction)cancelButtonTouched:(id)sender {
    [self hideKeyboard];
    if (_closeAlertBlock != nil) {
        _closeAlertBlock();
    }
}


/**
 IBAction method which allows the changed values of password and websiteurls to be saved to the repository.
 */
- (IBAction)saveButtonTouched:(id)sender {
    [self hideKeyboard];
    if (_passwaordSaveAlertBlock != nil) {
        
        if (_passwordTextFiled && _passwordTextFiled.text.length > 0) {
            if (_passwordTextFiled.text.length > MAXLENGTH_PASSWORD) {
                [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!") andMessage:[NSString stringWithFormat:NSLocalizedString(@"Password should allow up to 50 characters only", @"Password should allow up to 50 characters only"), self.headerLable.text]];
                return;
            }

            _passwaordSaveAlertBlock(_passwordTextFiled.text,self.headerLable.text, _websiteTextField.text);
        } else {
            [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!") andMessage:NSLocalizedString(@"Password cannot be empty", @"Password cannot be empty")];
        }
    }
}


/**
 IBAction method which handles the action on tapping remove button from removepassword type of alertview.
 */
- (IBAction)removePasswordButtonTouched:(id)sender {
    if (_removePasswaordAlertBlock != nil) {
        _removePasswaordAlertBlock(self.headerLable.text);
    }
}

/**
 IBAction method which handles the action on tapping remove all passwords button from removeallpasswords type of alertview.
 */
- (IBAction)removeAllPasswordButtonTouched:(id)sender {
    if (_removeAllPasswaordAlertBlock != nil) {
        _removeAllPasswaordAlertBlock();
    }
}

/**
 IBAction method which handles the action on tapping Unauthorize button from UnauthorizeNymi type of alertview.
 */
- (IBAction)unauthorizeNymiButtonTouched:(id)sender {
    if (_unauthorizeNymiAlertBlock != nil) {
        _unauthorizeNymiAlertBlock();
    }
}

/**
 This method helps in hiding the keyboard after successfully entering the data.
 */
-(void)hideKeyboard {
    [self.passwordTextFiled resignFirstResponder];
    [self.websiteTextField resignFirstResponder];
}
@end
