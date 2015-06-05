//
//  Constants.h
//  BioNym
//
//  Created by Satyanarayana SVV on 12/17/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

/**
 The header file of the application which holds all the constant values and constant strings which are used across the application..
 */



#pragma  mark - Strings


#define NYMULATOR_IP        @"127.0.0.1"  /**<system ip to work with nymulator*/

#define APPLICATION_NAME    "PasswordVault-iOS" /**<name of the application used in NCL initialization*/
#define MAXLENGTH_PASSWORD  50
#define LOADING_DELAY       0.1
#define NCL_TIMER_DURATION  30


/**
 This section deals with the error handling part of the application.
 */
#pragma mark - Error handling

#define ERR_DOMAIN                  @"com.nymi" /** < specifies the error is in the ncl domain*/
#define HANDLER                     @"HandlerError" /** < specifies the error is in the handler*/


/**
 \enum enum NSFetchedResultsChangeType : NSUInteger
 Enum to specify where the flow is broken.
 */

NS_ENUM(NSUInteger, NCLError) {
    kNCLInitError = 101,
    kNCLDiscoveryError,
    kNCLAgreeError, /**< error while NClagree message is sent to NCL. */
    kNCLProvisionError, /**< error while NCLProvision message is sent to NCL. */
    kNCLValidationError
};





/**
 This section deals with the password saving, fetching and deleting functionalities
 */
#pragma mark - Password Handling

#define PROV_ID     @"ProvisionID" /** provisionid of the provisioned Nymi device. */
#define PROV_KEY    @"ProvisionKey" /** Provisionkey of the provisioned Nymi device. */
#define ENTRIES     @"Entries" /** */
#define TAG         @"Tag" /** key for Tag value of the saved password. */
#define PWD         @"Password" /** key for password value of the saved password */
#define WEBSITE_URL @"WebsiteURL"
#define STAR        @"Star" /** key for the star value of the saved password */
#define CELL_ITEM   @"ITEM" /** key for identifier of the cell */
#define CELL_HEADER @"HEADER" /** key for header cell identifier */
#define CELL_FOOTER @"FOOTER" /** key for footer cell identifier */

#define VERSION             @"CFBundleShortVersionString"/** key for getting the version of the application */
#define VERSION_PLACEHOLDER @"Version: %@" /** The string which displays the version info in the menuviewcontroller. */

/**
 This section handles the color codes of the colors used through out the application.
 */
#pragma mark - Colors

#define DARK_NAVY   @"#1D2E3E"  /**<dark sidebar background, top bar background */
#define LIGHT_NAVY  @"#314250"  /**<light sidebar background*/
#define WHITE       @"#FFFFFF"  /**<text on sidebar*/
#define DARK_WHITE  @"#8d8f99"  /**<lighter text on sidebar*/
#define DARK_TEAL   @"#017C8A"  /**<text field border*/
#define ORANGE      @"#DF7F3C"  /**<orange button, orange text*/
#define ORANGE_NORMALSATE @"#dc6d27" /**<orage for normal button state*/
#define ORANGE_SELECTEDSATE @"#c95d28" /**<orage for selected button state*/
#define HIGH_LIGHT_COLOR @"#E8E7D8"
#define GRAY        @"#6D6C66"  /**<grey buttons, main text*/

/**
 This section holds the names of the segues used in this application.
 */

#pragma  mark - Segues

#define AGREEPROVISION_SEGUE    @"AGREE_PROVISION" /** < The segues gets triggered after receiving the agreecode from Ncl. this segue navigates from PairingViewController to ConfirmationViewController */
#define CONTAINER_SEGUE         @"CONTAINER" /** <The Segue which navigates the control from ValidateViewController to ContainerView.  */
#define CONTAINER_IPAD_SEGUE         @"CONTAINER_IPAD" /** <The Segue which navigates the control from ValidateViewController to ContainerView-iPad.  */
#define MAINVIEW_SEGUE          @"MAINVIEW" /** < The identifier which helps the viewcontroller to instantiate the object of MainViewController.*/
#define MENUVIEW_SEGUE          @"MENUVIEW" /** < The identifier which helps the viewcontroller to instantiate the object of MenuViewController.  */
#define VALIDATE_SEGUE          @"VALIDATE" /** < the segue which navigates the control from confirmation view to validation view. */
#define ADDPASSWORD_SEGUE       @"ADDPASSWORD" /** < This segue triggers the action of presenting the ADDPASSWORD screen from MainviewController. */
#define ALERTVIEW               @"PMAlertView"
#define ALERVIEWCONTROLLER_ID   @"ALERTVIEW"   /** Identifier for AlertViewCOntroller */
#define POPUP_SEGUE             @"POPUP"   /** This segue triggers the action of creation of OptionViewController */
#define INITIAL_SETUP_SEGUE     @"INITIAL_SETUP" /** Segue that is used to display Setup screen */
#define INITIAL_VALIDATION_SEGUE @"INITIAL_VALIDATION" /** Segue that is used to dispay Validation screen */
#define COPY_TO_CLIPBOARD_SEGUE @"COPY_TO_CLIPBOARD"
#define VIEW_PASSWORD_SEGUE     @"VIEW_PASSWORD" /** Segue to show the view for displaying password*/
#define CHANGE_PASSWORD_SEGUE   @"CHANGE_PASSWORD" /** Segue to show the view for changing the password*/
#define OPEN_URL_SEGUE          @"OPEN_URL"
#define REMOVE_SEGUE            @"REMOVE" /** Segue to show the view for removing segue*/
#define WARNING_VIEW            @"WARNINGVIEW" /** Segue to show the view for warning */

/**
 This section holds the messages of the NCL in this application.
 */
#pragma mark - messages

#define DISCONNECTION_LOCAL                 "NCL_DISCONNECTION_LOCAL" /** < describs the disconection reason to be local.   */
#define DISCONNECTION_TIMEOUT               "NCL_DISCONNECTION_TIMEOUT" /** < describs the disconnection reason to be timeout.  */
#define DISCONNECTION_FAILURE               "NCL_DISCONNECTION_FAILURE" /** < describs the disconnection reason to be failure.  */
#define DISCONNECTION_REMOTE                "NCL_DISCONNECTION_REMOTE" /** < describs the disconnection reason to be remote.  */
#define DISCONNECTION_CONNECTION_TIMEOUT    "NCL_DISCONNECTION_CONNECTION_TIMEOUT" /** < describs the disconnection reason to be connection time out.  */
#define DISCONNECTION_RESPONSE_TIMEOUT      "NCL_DISCONNECTION_LL_RESPONSE_TIMEOUT" /** < describs the  disconnection reason to be response timeout. */
#define DISCONNECTION_OTHERS                "NCL_DISCONNECTION_OTHER" /** < describs the disconnection reason to be something else.  */
#define DISCONNECTION_INVALID               "invalid disconnection reason, something bad happened" /** < describs the disconnection reason to be something bad had happened.  */

#define NCL_INIT_FAILED                     @"NCL_INIT_FAILED" /** < when  nclInit method returns error.this message will be sent to the view controller. */
#define ERROR_TAG                           @"ERROR"  /** < NCLWrapper class adds error messages with this key */

#define DATA_TAG                            @"DATA" /** < NCLWrapper class adds data(Agreecode, provisionkey and provisionid) which needs to be transferd to the view controller will be set using this key */

/**
 Debug log - Logs the messages generated by app only in debug mode.
 */
#ifdef DEBUG
#   define DLog(fmt, ...) NSLog((fmt), ##__VA_ARGS__);
#else
#   define DLog(...)
#endif

/**
 Images
 */
#pragma mark - Images

#define FAILURE_IMAGE [UIImage imageNamed:@"FailureMark"]
#define RETRY_IMAGE [UIImage imageNamed:@"RetryButtonImage"]
#define SUCCESS_IMAGE [UIImage imageNamed:@"Tickmark"]
#define TRANSPARENT_IMAGE [UIImage imageNamed:@"Transparent"]
#define STAR_ON [UIImage imageNamed:@"StarOn"]
#define STAR_OFF [UIImage imageNamed:@"StarOff"]

/**
 Device detection
 */
#define IS_IPAD (UI_USER_INTERFACE_IDIOM() != UIUserInterfaceIdiomPhone)