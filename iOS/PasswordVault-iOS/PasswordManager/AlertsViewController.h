//
//  AlertsViewController.h
//  PasswordVault
//
//  Created by Satyanarayana on 01/01/15.
//  Copyright (c) 2015 Nymi Inc. All rights reserved.
//

#import "PMAlertView.h"


@protocol AlertHandlerDelegate <NSObject>

/**This method tells the implementing class to handle the deletion of userdata specific to the user.*/
- (void)didUserUnAuthorized;

/**
 
 This method tells the implemeting class that the user has canceled the popup. 
 */
- (void)cancelPopUP;

@end

/**
 \brief The AlertsViewController class is a sub-class on OptionsBaseViewController, which is responsible for creating the alertview with options.
 
 Its a sub class to OptionsBaseViewController which creates the different options in a alertview based on the alertType value.
 */

@interface AlertsViewController : UIViewController
/**
 \property AlertType alertType
 \brief the property which holds the current alert type of the alert view.
 */
@property (nonatomic) PMAlertType alertViewType;

@property (nonatomic,assign) id<AlertHandlerDelegate> delegate;


@end
