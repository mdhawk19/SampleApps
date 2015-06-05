//
//  MenuViewController.m
//  BioNym
//
//  Created by Satyanarayana SVV on 12/18/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "MenuViewController.h"
#import "Constants.h"
#import "Utility.h"
#import "PKRevealController.h"

#define SHOW_HIDE_OPTIONS_ANIMATION 0.3
#define OPTIONS_HIDE_ALPHA 0.0
#define OPTIONS_SHOW_ALPHA 1.0

@interface MenuViewController ()

@property (weak, nonatomic) IBOutlet UIButton *clearPasswords;
@property (weak, nonatomic) IBOutlet UIButton *unAuthorize;
@property (weak, nonatomic) IBOutlet UIButton *addNew;

@property (nonatomic) BOOL optionsHidden;

@property (weak, nonatomic) IBOutlet UIView *topView;
@property (nonatomic, weak) IBOutlet UILabel* version;

@end

@implementation MenuViewController

/**
 This method is part of view's life cycle. It will be called as soon as controller's view is loaded into memory. This method will be called regardless of wheter view is loaded from XIB or storyboard or created programatically. You usually override this method to perform additional initialization on views that were loaded from nib files.
 
 Customize UI
 */
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.topView.backgroundColor = [Utility colorFromHexString:DARK_NAVY];
    self.view.backgroundColor = [Utility colorFromHexString:LIGHT_NAVY];
    
    self.addNew.backgroundColor = [Utility colorFromHexString:ORANGE];
    self.addNew.layer.cornerRadius = 5;
    
    self.optionsHidden = YES;
}

/**
 This method is part of View's life cycle. As soon as Controller's view is added to view hierarcy, controller will be notified.
 
 You can over ride this method to handle any of the UI customization or any other task associated with it. You must call "super" at some point of implementation.
 */
-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    NSString * version = [[NSBundle mainBundle] objectForInfoDictionaryKey:VERSION];
    self.version.text = [NSString stringWithFormat:VERSION_PLACEHOLDER, version];
}

/**
 When the app receives memory warning, this notification will be sent to view controller. You should never call this method directly. When the system determines that the app is occupying more memory than allocated, it will throw a notification "available memory is low"
 
 In this method you can release any of the objects that are taking more memory like arrays etc.
 */
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/**
 Action performed on tapping "Add New" button in Menu.
 This will call the delegate method "addNewEntry"
  Before performing the action, it will hide the menu.
 @param sender sender object, here the button that is tapped
 */
- (IBAction)addNewItem:(id)sender {
    [self hideMenu];
    [self.delegate addNewEntry];
}

/**
 Action performed to show all the Entries made by user in the Labels listing screen.
 This will call the deleage method "showEnteries" with "YES" are parameter.
  Before performing the action, it will hide the menu.
 @param sender sender object, here the button that is tapped
 */
- (IBAction)showAll:(id)sender {
    [self hideMenu];
    [self.delegate showEntries:YES];
}

/**
 Action performed to filter the entries that are marked as "Star" by user in the Labels listing screen.
 This will call the deleage method "showEnteries" with "NO" are parameter.
  Before performing the action, it will hide the menu.
 @param sender sender object, here the button that is tapped
 */
- (IBAction)showStarred:(id)sender {
    [self hideMenu];
    [self.delegate showEntries:NO];
}

/**
 Two options are avaiable in the Menu. One is Clear all passwords and the other one is UnAutorize the Nymi band. 
  Before performing the action, it will hide the menu.
  @param sender sender object, here the button that is tapped
 */
- (IBAction)showHideOptions:(id)sender {
    self.optionsHidden = !self.optionsHidden;
    [UIView animateWithDuration:SHOW_HIDE_OPTIONS_ANIMATION animations:^{
        self.clearPasswords.alpha = self.unAuthorize.alpha = self.optionsHidden ? OPTIONS_HIDE_ALPHA : OPTIONS_SHOW_ALPHA;
    }];

}

/**
 Action perform to clear all the Labels/passwords created by user.
 This will call the delegate method "clearAllPasswords"
  Before performing the action, it will hide the menu.
  @param sender sender object, here the button that is tapped
*/
- (IBAction)clearAllPasswords:(id)sender {
    [self hideMenu];
    [self.delegate clearAllPasswords];
}


/**
 Action perform to clear all the Labels/passwords created by user and unauthorize the Nymi band.
 This will call the delegate method "unAuthorize"
 Before performing the action, it will hide the menu.
 @param sender sender object, here the button that is tapped
 */
- (IBAction)unauthorizeUser:(id)sender {
    [self hideMenu];
    [self.delegate unAuthorize];
}

/**
 Upon performing some action in Menu like filtering Labels etc, this method will be called to hide the menu and take the user to Listing labels screen.
 */
-(void)hideMenu {
    PKRevealController* rv = self.revealController;
    [rv toggleMenu];
}

@end
