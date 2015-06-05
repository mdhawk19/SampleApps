//
//  ItemCell.h
//  PasswordVault
//
//  Created by Satyanarayana on 30/12/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <UIKit/UIKit.h>


/**
 @protocol ItemCellDelegate "PasswordManager/ItemCell.h"
 \brief The ItemCellDelegate protocol handles selection of item from the Tableview.
 
 This Protocol Has method to specify user selected rom in the tableview..
 */
@protocol ItemCellDelegate <NSObject>

/**
 This function gives the implemeting class about the selected cell in a tableview.
 @param index index of the selected row.
 @return nil.
 */
-(void)itemTappedAtIndex:(NSInteger)index;

@end



/**
 \brief The ItemCell class is a sub-class on UITableViewCell, which Creates thetableview cell for the table.
 
 Thisobject is responsible for look and feel of the cell and handling of selectiopn in table view.
 */

@interface ItemCell : UITableViewCell


/**
 \property index
 \brief The value of the index for the current row.
 */
@property(nonatomic) NSInteger index;

/**
 \property star
 \brief the value of the start for the current row.
 
 In the view password page each password will have a start value which is represented by this property.
 */
@property(nonatomic, strong) IBOutlet UIButton* star;

/**
 \property itemTag
 \brief the value of the tag for the current row.
 
 In the view password page each password will have a tag value which is represented by this property.
 */
@property(nonatomic, strong) IBOutlet UILabel* itemTag;


/**
 \property delegate
 \brief the delegate class which will implement the ItemCellDelegate protocol.
 
 */
@property(nonatomic, weak) id<ItemCellDelegate> delegate;

@end
