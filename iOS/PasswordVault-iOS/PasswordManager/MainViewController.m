//
//  MainViewController.m
//  PasswordVault
//
//  Created by Satyanarayana on 30/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "MainViewController.h"
#import "ItemCell.h"
#import "Utility.h"
#import "MenuViewController.h"
#import "PasswordManager.h"
#import "AddPasswordViewController.h"
#import "OptionsViewController.h"
#import "PMAlertView.h"
#import "AlertsViewController.h"
#import "ValidateViewController.h"
#import "NCLWrapper.h"
#import "AESCrypt.h"
#import "PKRevealController.h"


@interface MainViewController () <UIGestureRecognizerDelegate, OptionsDelegate, ItemCellDelegate,AlertHandlerDelegate>

@property (nonatomic, strong) UIView* menuView;
@property (nonatomic, strong) NSArray* allTags;

@property (weak, nonatomic) IBOutlet UITableView *tagsList;
@property (weak, nonatomic) IBOutlet UIView *optionsView;
@property (weak, nonatomic) OptionsViewController *optionsViewController;
@property (nonatomic, strong) PMAlertView *alert;
@property (nonatomic) BOOL firstTime;
@property (nonatomic) BOOL isBusyHandlingOptions;
@property (nonatomic, strong) AlertsViewController *alertsViewController;
@property (weak, nonatomic) IBOutlet UIView *header;
@property (weak, nonatomic) IBOutlet UIButton *slideMenuButton;

@end

@implementation MainViewController

/**
 This method is part of view's life cycle. It will be called as soon as controller's view is loaded into memory. This method will be called regardless of wheter view is loaded from XIB or storyboard or created programatically. You usually override this method to perform additional initialization on views that were loaded from nib files.
 
 For the first tme when the view loads, it has to check if any Labels are already entered or not. If its not entered, user has to be provided "Add Password" screen. So to track that "firstTime" boolean variable will be set to YES.
 */
-(void)viewDidLoad {
    [super viewDidLoad];
    self.header.backgroundColor = self.view.backgroundColor = [Utility colorFromHexString:DARK_NAVY];
    self.firstTime = YES;
    if (IS_IPAD) {
        self.slideMenuButton.hidden = YES;
    }
}

/**
 This method is part of View's life cycle. As soon as Controller's view is added to view hierarcy, controller will be notified.
 
 You can over ride this method to handle any of the UI customization or any other task associated with it. You must call "super" at some point of implementation.
 */
-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self updateList];
}

/**
 This method is part of view's life cycle. Notifies the view controller that its view was added to a view hierarchy. One can over ride this method to handle the tasks (like updating the UI etc) associated while showing the view. Its mandatory to call "super" when over riding.
 
 If this view is being shown for the first time after the app starts, and if there are no Labels/entries added by user, it will display Add password screen.
 
 @parm animated If YES, the view was added to the window using an animation.
 */
-(void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    if (self.firstTime && !self.allTags.count) {
        [self performSegueWithIdentifier:ADDPASSWORD_SEGUE sender:nil];
    }
    self.firstTime = NO;
}

/**
 Updates the list of Labels from the local database. It fetches all the Labels, sors them alphabetically and then update the UI accordingly.
 If the text length is more than the available space, then the text is truncated and adds ... at the left most end.
 */
-(void)updateList {
    self.allTags = [PasswordManager getAllTags];
    [self sortAndLoad];
}

/**
 Whenever an update happens on the Listing, this methos will take the updated list and sorts the list to show the sorted list on the view.
 */
-(void)sortAndLoad {
    NSMutableArray *genericArray = [NSMutableArray arrayWithArray:self.allTags];
    
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:TAG ascending:YES
                                                                    selector:@selector(localizedCaseInsensitiveCompare:)];
    
    NSArray *discriptor = [NSArray arrayWithObject:sortDescriptor];
    self.allTags = [genericArray sortedArrayUsingDescriptors:discriptor];
    
    [self.tagsList reloadData];
}

/**
 This method is related to UITableview's data source protocol that tells the data source to return the number of rows in a given section of a table view.
 Number of rows will be equal to number of labels already added by user.
 
 @param tableView The table-view object requesting this information.
 @param section An index number identifying a section in tableView.
 
 @return Number of labels added by user
 */
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.allTags.count;
}

/**
 This method is related to UITableView's data source protocol that asks the data source for a cell to insert in a particular location of the table view.
 The cell returned by this method is re-used to improve the performance of the app. You must fetch previously created cell by using "dequeueReusableCellWithIdentifier" and use that. While using storyboard, if the cell is not created earlier, it will create a new one and will be provied by table view.
 ItemCell is the custom table view cell created to handle the UI as per the design.
 Update the table view cell with the label and set the "star" image accoring to the status of "Starred" or not.
 
 @param tableView A table-view object requesting the cell.
 @param indexPath An index path locating a row in tableView.
 
 @return Updaed ItemCell object
 */
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString* cellID = CELL_ITEM;
    ItemCell* cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    cell.delegate = self;
    
    NSDictionary* tag = self.allTags[indexPath.row];
    if ([tag[STAR] boolValue]) {
        [cell.star setImage:STAR_ON forState:UIControlStateNormal];
    } else {
        [cell.star setImage:STAR_OFF forState:UIControlStateNormal];
    }
    
    cell.index = indexPath.row;
    cell.itemTag.text = tag[TAG];
    
    return cell;
}

/**
 This is table view's delegate method that will be called when user taps on the row
 */
- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    
    self.isBusyHandlingOptions = YES;
    self.optionsView.hidden = NO;
    [self.tagsList deselectRowAtIndexPath:indexPath animated:NO];
    
    PasswordManager *passwordManager = [PasswordManager sharedInstance];
    passwordManager.selectedPassword = [self.allTags objectAtIndex:indexPath.row];
    
    self.optionsViewController.tagLabel.text = [[self.allTags objectAtIndex:indexPath.row] valueForKey:TAG];
    NSString *removeTitle = [NSString stringWithFormat:@"Remove %@",passwordManager.selectedPassword[TAG]];
    
    [self.optionsViewController.removeButton setTitle:removeTitle forState:UIControlStateNormal];
}

/**
 This is table view's delegate method wich will create the footer view for the table. In this view we have a AddPassword button, using which user can craete a new password.
 */
- (UIView *)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section {
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:CELL_FOOTER];
    return cell;
}

/**
 The iPhone has a swipable menu view in the left which animates on the user swipe action. To understand the user that a MenuView is hidden behind the MainView, we have given a Button whose action is mapped to this method.
 This is an action mapped from the storyboard swipebutton which is available only in iPhone which enable the swipe action on the MainView.
 @param id sender the instance of the button which has triggered the action.
 */
- (IBAction)swipeButtonAction:(id)sender {
    
    PKRevealController* pkRevealController = self.revealController;
    [pkRevealController toggleMenu];
}

/**
 The table view which lista all the labels has a footerview with Add Password button. This button allows the user to add new password by presenting AddNewPassword page. the same action is been handled by this method.
 @param id sender the instance of the button which has triggered the action.
 */
- (IBAction)addPasswordFromFooter:(id)sender {
    [self addNewEntry];
}


- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    if ([gestureRecognizer.view isEqual:self.view] && [otherGestureRecognizer.view isEqual:self.tagsList]) {
        return NO;
    }
    return YES;
}

/**
 When the user taps on any of the Label, the options related to selected label such as View Password, Copy Password, Change password, OpenURl, Remove Password  will be shown with the help of OptionViewController. When the user taps outside of this view the same will be dismissed. To handle the dismiss action this method will get called.
 */
- (void)hideOptions {
    self.isBusyHandlingOptions = NO;
    self.optionsView.hidden = YES;
    [self updateList];
    
    PasswordManager *passwordManager = [PasswordManager sharedInstance];
    [passwordManager setSelectedPassword:@{}];
}

/**
 This method handles loading of saved URL.
 When user selects Launch URl option from the selected Label, application validates the saved url and if the url is valid then it will load the url else a proper error message will be shown to the user. while laoding it copies the selected password to the clipboard as well.
 */
-(void)openLabelURL {
    NSString* url = [[[PasswordManager sharedInstance] selectedPassword] objectForKey:WEBSITE_URL];
    NSString *password = [[[PasswordManager sharedInstance] selectedPassword] objectForKey:PWD];
    password = [AESCrypt decrypt:password];
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    [pasteboard setString:password];
    
    if ( url && url.length) {
        if (![url hasPrefix:@"http"]) {
            url = [NSString stringWithFormat:@"http://%@",url];
        }
        
        if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:url]]) {
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
        } else {
            [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!")  andMessage:NSLocalizedString(@"There's no URL associated with this Label", @"There's no URL associated with this Label")];
        }
    } else {
        [Utility showAlertWithTitle:NSLocalizedString(@"Uh oh!", @"Uh oh!")  andMessage:NSLocalizedString(@"Can't open this url.", @"Can't open this url.")];
    }
    
    self.isBusyHandlingOptions = NO;
    self.optionsView.hidden = YES;
    PasswordManager *passwordManager = [PasswordManager sharedInstance];
    [passwordManager setSelectedPassword:@{}];
}

/**
 This is a Delegate method from ItemCell class which will get called when the user taps star button present on it.
 This method allows the user to start/unstar the label and updates the list after completing the action.
 
 @param NSInteger index the index of the label which needs to be updated for star/unstaring.
 */
-(void)itemTappedAtIndex:(NSInteger)index {
    [PasswordManager starUnStartPasswordwithTag:[[self.allTags objectAtIndex:index] valueForKey:TAG]];
    [self updateList];
}

/**
 The method witch handles the action of Unauthorization of user upon confirmation from the user. it clears all the data with respect to the user and allows the user setup the application again.
 */
- (void)didUserUnAuthorized {
    [self cancelPopUP];
    [self.navigationController popToRootViewControllerAnimated:YES];
}

/**
 This method will allows the user to close the popup.
 */
- (void)cancelPopUP{
    self.isBusyHandlingOptions = NO;
    [self.alertsViewController willMoveToParentViewController:nil];
    [self.alertsViewController.view removeFromSuperview];
    [self.alertsViewController removeFromParentViewController];
    self.alertsViewController = nil;
    [self updateList];
}


/**
 This method takes the user to the Add New Password page with the help of ADDPASSWORD_SEGUE.
 
 */
- (void)addNewEntry {
    if (self.isBusyHandlingOptions) {
        return;
    }
    [self performSegueWithIdentifier:ADDPASSWORD_SEGUE sender:nil];
}


/**
 This method helps in filetring the list of Labels. if the value of showAll is true then it shows all the labels else it shows only starred labels.
 
 @param BOOL showAll the boolean value which decides wheather all the passwords needs to be shown or only the starred labels.
 
 */
- (void)showEntries:(BOOL)showAll {
    if (self.isBusyHandlingOptions) {
        return;
    }
    
    if (showAll) {
        [self updateList];
    }else {
        NSArray *list = [PasswordManager getAllTags];
        NSIndexSet* indexSet = [list indexesOfObjectsPassingTest:^BOOL(id obj, NSUInteger idx, BOOL *stop) {
            return [obj[STAR] boolValue];
        }];
        
        self.allTags = [list objectsAtIndexes:indexSet];
        [self sortAndLoad];
    }
}


/**
 The delagate method from MenuView which gets called when user taps clear all passwords option from MenuView.
 This method presents the user with confirmation popup about the action.
 */
- (void)clearAllPasswords {
    [self showAlertViewControllerForType:PMAlertTypeRemoveAllPassword];
}

/**
 The delagate method from MenuView which gets called when user taps Unauthorize user option from MenuView.
 This method presents the user with confirmation popup about the action.
 */
- (void)unAuthorize {
    [self showAlertViewControllerForType:PMAlertTypeUnauthorizeNymi];
}


/**
 This method creates the popup view with specific alerttype based on the value specified in the input type alert.
 This method creates the instance of AlertViewController and populates the data based on the slerttype.
 
 @param PMAlertType alert the parameter which decides which type of popup needs to be craeted.
 */
-(void)showAlertViewControllerForType:(PMAlertType)alert {
    
    if (self.isBusyHandlingOptions || self.alertsViewController ) {
        return;
    }
    
    self.alertsViewController = [self.storyboard instantiateViewControllerWithIdentifier:ALERVIEWCONTROLLER_ID];
    self.alertsViewController.alertViewType = alert;
    self.alertsViewController.delegate = self;
    
    [self addChildViewController:self.alertsViewController];
    self.alertsViewController.view.translatesAutoresizingMaskIntoConstraints = NO;
    self.alertsViewController.view.frame = self.view.bounds;
    [self.view addSubview:self.alertsViewController.view];
    
    NSLayoutConstraint *width =[NSLayoutConstraint
                                constraintWithItem:self.alertsViewController.view
                                attribute:NSLayoutAttributeWidth
                                relatedBy:0
                                toItem:self.view
                                attribute:NSLayoutAttributeWidth
                                multiplier:1.0
                                constant:20];
    NSLayoutConstraint *height =[NSLayoutConstraint
                                 constraintWithItem:self.alertsViewController.view
                                 attribute:NSLayoutAttributeHeight
                                 relatedBy:0
                                 toItem:self.view
                                 attribute:NSLayoutAttributeHeight
                                 multiplier:1.0
                                 constant:0];
    NSLayoutConstraint *top = [NSLayoutConstraint
                               constraintWithItem:self.alertsViewController.view
                               attribute:NSLayoutAttributeTop
                               relatedBy:NSLayoutRelationEqual
                               toItem:self.view
                               attribute:NSLayoutAttributeTop
                               multiplier:1.0f
                               constant:0.f];
    NSLayoutConstraint *leading = [NSLayoutConstraint
                                   constraintWithItem:self.alertsViewController.view
                                   attribute:NSLayoutAttributeLeading
                                   relatedBy:NSLayoutRelationEqual
                                   toItem:self.view
                                   attribute:NSLayoutAttributeLeading
                                   multiplier:1.0f
                                   constant:0.f];
    [self.view addConstraint:width];
    [self.view addConstraint:height];
    [self.view addConstraint:top];
    [self.view addConstraint:leading];
    
    [self.alertsViewController didMoveToParentViewController:self];
    [self.view bringSubviewToFront:self.alertsViewController.view];
}

/**
 This method will be used when you use "storyboard" in your application. As and when a new view controller is going to be presented, system will notify the view controller about the new "segue" to be performed.
 
 You have to over ride this method when you have to pass some data to destination view controller that is going to be displayed.
 
 Set the delegate of Options view controller to self so as to handle the call backs.
 
 @param segue The segue object containing information about the view controllers involved in the segue.
 @param sender The object that initiated the segue. You might use this parameter to perform different actions based on which control (or other object) initiated the segue.
 */
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([segue.identifier isEqualToString:POPUP_SEGUE]) {
        UINavigationController *optionsNavigationController = segue.destinationViewController;
        self.optionsViewController = (OptionsViewController *)[[optionsNavigationController viewControllers] objectAtIndex:0];
        self.optionsViewController.delegate = self;
    }
}


@end