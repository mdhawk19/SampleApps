//
//  NCLWrapper.h
//  BioNym
//
//  Created by Satyanarayana SVV on 11/21/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Constants.h"


/**
 \brief 
 NCLWrapper class as a connctor between your application and the NCL library. NCL Library is implemented in C language. NCL Wrapper is implemented in objective C to make the underlying C language calls in order to communicate with Nymulator/Nymi band.
 
 This class provides the call backs in form of blocks that are asynchronous in nature.
 
 This class handles below functions related to NCL library:
    a) Initialization of NCL library (based on Nymulator or Nymi band)
    b) Discovering the near by device
    c) Getting the agree code that represents the status of LEDs for synchronization
    d) Provisioning the device
    e) Validating the device with the provisioned ID
 
 This is a singleton class that completely handles the intermediate values exchanged between different states of Nymi band/Nymulator
 */


/**
 \typedef typedef void (^NCLHandlerWithError) (id, NSError*);
 Blocks are a language-level feature added to Objective-C which allow you to create distinct segments of code that can be passed around to methods or functions as if they were values. Blocks are Objective-C objects, which means they can be added to collections like NSArray or NSDictionary. They also have the ability to capture values from the enclosing scope, making them similar to closures or lambdas in other programming languages.
 
 This is the definition of "block" that will be used as response to the caller of this wrapper.
 
 The response from the NCL library call back will be passed back to the application in form of NSArray or NSDictionary which is represented as "id". If the call back gives successful response, error will be nil. Else appropriate error will be conveyed to the app
 @param id which holds the data of the action.
 @param NSError which holds the error object.
 */
typedef void (^NCLHandlerWithError) (id, NSError*);


@interface NCLWrapper : NSObject

/**
 \property NSInteger scanTimeOut
 Actions like Discovery and Finding Nymi band/Nymulator will be performed before provisioning and validating them. During this process, if there's no active device near by, the app will go into continous mode and keeps on search for valid one. Set the scanTimeout that allows the app to search only for some amount of time.
 
 Default value is 30 seconds. Keep it less than 1 minute for better performance.
 */
@property(nonatomic) NSInteger scanTimeOut;


/**
 This static method helps to wrapper class to function as Sigalton instance.

 @return singalton instance of the NCLWrapper class.
 */
+(id)sharedInstance;

/**
 This method initiates the NCL lib rary, upon succesful initialization, starts discovering Nymi band/Nymulator in the visibility. When a device is discovered in its vicinity before the scan duration elapses, it will request for ageeCode that represents the LEDs signature of the device. This agree code will be sent back to the application so that the LED images in the app will be turned on/off accordingly.
 
 Valid response from NCL library will return NSDictionary object that will have event.agreement
 
 A timer is needed to initialize so as to restrict the discovery action active only for a limited duration. If there  are no Nymi bands or Nymulator found, it has to throw the error back to the caller.
 
 @param block The block that will return appropriate response or error based on response from NCL library. If the response is postive from NCL library, it returns proper formatted object and error will be nil. Else it will send NSError object with appropriate reason for failure.
 
 @return nil.
 */
-(void) discoverNymi:(NCLHandlerWithError)block;


/**
 This method will start provisioning the discovered Nymi.
 
 @param block The block that will return appropriate response or error based on response from NCL library. If the response is postive from NCL library, it returns proper formatted object and error will be nil. Else it will send NSError object with appropriate reason for failure.
 @return nil.
 */
-(void) provisionNymi:(NCLHandlerWithError)block;

/**
 Once the Nymi band/Nymulator is successfully provisioned, it has to be validated for further use of the Library.
 
 @param block The block that will return appropriate response or error based on response from NCL library. If the response is postive from NCL library, it returns proper formatted object and error will be nil. Else it will send NSError object with appropriate reason for failure.
 
 @return nil.
 */
-(void) findAndValidateNymi:(NCLHandlerWithError)block;

/**
 This method informs the NCL to stop further scanning of the Nymi Device.
 */
-(void) stopScanningNymi;

/**
 This function inform NCL to disconnect from the NEA which is currently connected.
 */
-(void) disconnectNymi;





@end
