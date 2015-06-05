//
//  NCLWrapper.m
//  BioNym
//
//  Created by Satyanarayana SVV on 11/21/14.
//  Copyright (c) 2014 Nymi Inc. All rights reserved.
//

#import "NCLWrapper.h"
#import "ncl.h"
#import "Constants.h"

void callback(NclEvent event, void* userData);

@interface NCLWrapper ()

@property (nonatomic) int nymiHandler;
@property (nonatomic, assign) BOOL nclInitStatus;
@property (nonatomic, assign) BOOL discoveryMode;
@property (nonatomic, assign) BOOL nclBandDiscovered;
@property (nonatomic, strong) NSTimer* scanTimer;
@property (copy) NCLHandlerWithError handlerWithError;

@end

@implementation NCLWrapper

/**
 This static method helps to wrapper class to function as Sigalton instance.
 
 @return singalton instance of the NCLWrapper class.
 */
+(id)sharedInstance {
    static dispatch_once_t onceToken;
    static NCLWrapper* wrapper = nil;
    dispatch_once(&onceToken, ^{
        wrapper = [[self alloc] init];
    });
    
    return wrapper;
}

/**
 Implemented by subclasses of NSObject to initialize a new object (the receiver) immediately after memory for it has been allocated.
 
 Set default values for the variables.
 */
-(id)init {
    self = [super init];
    if (self) {
        _scanTimeOut = NCL_TIMER_DURATION;
    }
    
    return self;
}

/**
 initNCL is the method that will be initializing the NCL library.
 There are two targets in this project. One is "PasswordVault Nymulator" and the other one is "PasswordVault Device". In the target "PasswordVault Nymulator" build settings, under Preprocessor Macros, "NYMULATOR" is defined. In the other target, its not defined.
 The reason for it is to set IP address and Port in order to work with Nymulator. Also nclInit method will have different parameters for initialization of NCL library.
 Make sure that NCL library has to beinitialized only once for life time of the App.
 */
-(void) initNCL {
    
#if NYMULATOR  //Following code has to be compiled to test the app with 
    NclBool result = nclSetIpAndPort([NYMULATOR_IP cStringUsingEncoding:NSUTF8StringEncoding], 9089);
    if (result) {
        result = nclInit(callback, (__bridge void*)self, APPLICATION_NAME, NCL_MODE_DEV, NULL);
        
        DLog(@"%@", [NSString stringWithFormat:@"about to nclInit %@",
                     result ? @"success" : @"failed"]);
        if (!result) {
            [self.scanTimer invalidate];
            [self sendError:kNCLInitError withUserInfo:NSLocalizedString(@"Initialization Error", @"Initialization Error")];
        }
    } else {
        DLog(@"nclSetIpAndPort failed. Please check the IP and make sure Nymulator is running");
        [self.scanTimer invalidate];
        [self sendError:kNCLInitError withUserInfo:NSLocalizedString(@"Initialization Error", @"Initialization Error")];
    }
#else
    NclBool result = nclInit(callback, (__bridge void*)self, APPLICATION_NAME, NCL_MODE_DEFAULT, stderr);
    DLog(@"%@", [NSString stringWithFormat:@"about to nclInit %@",
                 result ? @"success" : @"failed"]);
    
    if (!result) {
        [self.scanTimer invalidate];
        [self sendError:kNCLInitError withUserInfo:NSLocalizedString(@"Initialization Error", @"Initialization Error")];
    }
#endif
}

-(void)sendError:(int)code withUserInfo:(NSString*)info {
    NSError* err = [NSError errorWithDomain:ERR_DOMAIN code:code userInfo:@{HANDLER:info}];
    self.handlerWithError(nil, err);
}
/**
 This method initiates the NCL lib rary, upon succesful initialization, starts discovering Nymi band/Nymulator in the visibility. When a device is discovered in its vicinity before the scan duration elapses, it will request for ageeCode that represents the LEDs signature of the device. This agree code will be sent back to the application so that the LED images in the app will be turned on/off accordingly.
 
 Valid response from NCL library will return NSDictionary object that will have event.agreement
 
 A timer is needed to initialize so as to restrict the discovery action active only for a limited duration. If there  are no Nymi bands or Nymulator found, it has to throw the error back to the caller.
 
 @param block The block that will return appropriate response or error based on response from NCL library. If the response is postive from NCL library, it returns proper formatted object and error will be nil. Else it will send NSError object with appropriate reason for failure.
 
 @return nil.
 */
-(void) discoverNymi:(NCLHandlerWithError)block {
    NSAssert(block != nil, @"Handler cannot be nil");
    [self validateTimer];
    self.handlerWithError = block;
    self.discoveryMode = YES;
    
    if (self.nclInitStatus) {
        [self startDiscovery];
    } else {
        [self initNCL];
    }
}

-(void) validateTimer {
     DLog(@"Timer started...");
    self.scanTimer = [NSTimer scheduledTimerWithTimeInterval:self.scanTimeOut target:self selector:@selector(timeOutCallBack) userInfo:nil repeats:NO];
}

/**
 If the discovery of find operations dont complete in limited timer duration, it has to throw error back to the caller.
 */
-(void) timeOutCallBack {
    if (!self.nclBandDiscovered) {
        DLog(@"Timer stoped...");
        nclStopScan();
        NSError* err = [NSError errorWithDomain:ERR_DOMAIN code:kNCLAgreeError userInfo : @ { HANDLER:NSLocalizedString(@"We can't find your Nymi Band. Try tapping more rapidly and make sure your Nymi Band is activated.", @"We can't find your Nymi Band. Try tapping more rapidly and make sure your Nymi Band is activated.")}];
        self.handlerWithError(nil, err);
    }
}

/**
 This method will start provisioning the discovered Nymi.
 
 @param block The block that will return appropriate response or error based on response from NCL library. If the response is postive from NCL library, it returns proper formatted object and error will be nil. Else it will send NSError object with appropriate reason for failure.
 
 @return nil.
 */
-(void) provisionNymi:(NCLHandlerWithError)block {
    NSAssert(block != nil, @"Handler cannot be nil");
    self.handlerWithError = block;
    DLog(@"About to provision...");
    nclProvision(self.nymiHandler);
}

/**
 Once the Nymi band/Nymulator is successfully provisioned, it has to be validated for further use of the Library.
 
 @param block The block that will return appropriate response or error based on response from NCL library. If the response is postive from NCL library, it returns proper formatted object and error will be nil. Else it will send NSError object with appropriate reason for failure.
 
 @return nil.
 */
-(void) findAndValidateNymi:(NCLHandlerWithError)block {
    NSAssert(block != nil, @"Handler cannot be nil");
    [self validateTimer];
    self.handlerWithError = block;
    self.discoveryMode = NO;
    
    if (!self.nclInitStatus) {
        [self initNCL];
    }else {
        [self findAndValidate];
    }
}

/**
 Calls the NCL library's "nclStartDiscovery" that will start searching for near by Nymi band/Nymulator. On successful response, it will send a "callBack" that is registered during initialization.
 */
-(void) startDiscovery {
    DLog(@"About to discover...");
    nclStartDiscovery();
}

/**
 This method will fetch the provision key and provision ID and initiates "nclStartFinding" in NCL Library.
 */
-(void) findAndValidate {

    NSString* provisionKey = [[NSUserDefaults standardUserDefaults] valueForKey:PROV_KEY];
    NSString* provisionID = [[NSUserDefaults standardUserDefaults] valueForKey:PROV_ID];

    if (!provisionKey || !provisionKey.length || !provisionID || !provisionID.length) {
        [self sendError:kNCLValidationError withUserInfo:NSLocalizedString(@"Failed to Find and Validate", @"Failed to Find and Validate")];
        return;
    }
    
    NclProvision provision;
    NSString *provKey = stringToProvisonId(provisionKey);
    NSString *provID = stringToProvisonId(provisionID);
    const char *cstrID = [provID cStringUsingEncoding:NSASCIIStringEncoding];
    const char *cstrKey = [provKey cStringUsingEncoding:NSASCIIStringEncoding];
    
    for (int c =0; c<strlen(cstrID); c++) {
        provision.id[c] = cstrID[c];
        provision.key[c] = cstrKey[c];
    }
    
    DLog(@"About to find Nymi...");
    nclStartFinding(&provision, 1, NCL_FALSE);
}


#pragma mark - ObjC NCL callbacks

/**
 This is the call back received from NCL Library upon successful initialization of NCL library. 
 Upon successful initialization, it will initiate either discovery of unprovisioned Nymi band/Nymulator or find a provisioned Nymi band/Nymulator
 @param status NCL_FALSE if the initialization fails. NCL_TRUE if initialization is success.
 */
-(void) updateNCLInitStatus:(NclBool) status {
    self.nclInitStatus = status;

    DLog(status ? @"NCI Init Success..." : @"NCI Init failed.");
    if(self.nclInitStatus) {
        if (self.discoveryMode) {
            [self startDiscovery];
        }else {
            [self findAndValidate];
        }
    }else {
        NSError* err = [NSError errorWithDomain:ERR_DOMAIN code:kNCLInitError userInfo:@{HANDLER:NSLocalizedString(@"Failed to Initialize NCL Library", @"Failed to Initialize NCL Library")}];
        self.handlerWithError(nil, err);
    }
}

/**
 This is the call back from NCL library upon succesful discovery of nearby Nymi band/Nymulator
 Upon successful response, it will stop scanning for more devices and initiates nclAgree.
 @parm nymiHandler Int value representing the identifier of the Nymi band/Nymulator
 */
- (void) onNymiDiscovery:(int)nymiHandler {
    [self.scanTimer invalidate];
    nclStopScan();
    self.nymiHandler = nymiHandler;
    DLog(@"About to Agree Nymi format ....");
    nclAgree(self.nymiHandler);
}

/**
 This is the call back from NCL library upon successfully receiving agree code that represents signature of LEDs on the Nymi band/Nymulator. The same will has to be sent to the UI for updating it and to confirm the same with user.
 @param nymiAgreeCode String that represents the LEDs On/Off status
 @param nymiHandler Int that represents Nymi band/Nymmulator identifier
 */
- (void) doNymiAgreeWithCode:(NSString *)nymiAgreeCode withHandler:(int)nymiHandler {
    self.handlerWithError(@{ DATA_TAG : nymiAgreeCode }, nil);
}


- (void) nymiProvisionedWithKey:(NSString*)key id:(NSString*) value withProvision:(int) handle {
    DLog(@"%@", [NSString stringWithFormat:@"Nymi Provisioned with id %@",value]);
    
    if (handle == self.nymiHandler) {
        self.handlerWithError( @{ PROV_KEY : key, PROV_ID : value }, nil);
        
        [[NSUserDefaults standardUserDefaults] setObject:key forKey:PROV_KEY];
        [[NSUserDefaults standardUserDefaults] setObject:value forKey:PROV_ID];
        [[NSUserDefaults standardUserDefaults] synchronize];
        nclDisconnect(self.nymiHandler);
    } else {
        NSError* err = [NSError errorWithDomain:ERR_DOMAIN code:kNCLAgreeError userInfo:@{HANDLER:NSLocalizedString(@"Improper handler returned by Nymi after Provision call", @"Improper handler returned by Nymi after Provision call")}];
        self.handlerWithError(nil, err);
    }
}

/**
 Ths is the call back from NCL library when it finds the Nymi band/Nymulator matching the identifier
 @param handle Identifier with which the Nymi band/Nymulator has to be searched
 */
-(void) nymiFoundHandle:(int) handle {
    [self.scanTimer invalidate];
    nclStopScan();
    self.nymiHandler = handle;
    DLog(@"About to validate Nymi...");
    nclValidate(handle);
}

/**
 This is the call back from NCL library that will be sent when it successfully validates with Nymi band/Nymulator.
 @param handle Identifier with which the Nymi band/Nymulator is validated
 */
-(void) nymiValidated:(int) handle {
    [self.scanTimer invalidate];
    DLog(@"%@", [NSString stringWithFormat:@"Validated Nymi %d",handle]);
    if (handle == self.nymiHandler) {
        nclDisconnect(handle);
        self.handlerWithError(@YES, nil);
    } else {
        self.handlerWithError(@NO, nil);
    }
}

/**
 This the call back from NCL library when the Nymi band/Nymulator successfully disconnects.
 You can handle clean up when it gets diconnected.
 @param handle Identifier with which the Nymi band/Nymulator is validated
 @param reason Reason for disconnection.
 */
-(void) nymiDisconnected:(int) handle reason:(NclDisconnectionReason) reason {
    DLog(@"%@", [NSString stringWithFormat:@"Nymi Disconnected %s", disconnectionReasonToString(reason)]);
}

/**
 In order to stop scanning any time, call this method. It will call NCL library's nclStopScan
 */
-(void) stopScanningNymi {
    nclStopScan();
}

/**
 In order to disconnect Nymi band/Nymulator, call this method.
 */
-(void) disconnectNymi {
    nclDisconnect(self.nymiHandler);
}

#pragma mark - NCL Callbacks

/**
 Helper method to convert provision ID into NSString
 @param provisionId Provision ID provided by the Nymi band/Nymulator
 @return NSString Stroing format of NclProvisionId
 */
NSString* provisionIdToString(NclProvisionId provisionId){
    NSMutableString* result=[[NSMutableString alloc] init];
    for(unsigned i=0; i<NCL_PROVISION_ID_SIZE; ++i)
        [result appendFormat: @"%x ", provisionId[i]];
    return result;
}

/**
 Convert Provision ID in string format to NclProvisionId
 */
NSString* stringToProvisonId(NSString* str){
    NSMutableString * newString = [NSMutableString string];
    NSArray * components = [str componentsSeparatedByString:@" "];
    for ( NSString * component in components ) {
        int value = 0;
        sscanf([component cStringUsingEncoding:NSASCIIStringEncoding], "%x", &value);
        [newString appendFormat:@"%c", (char)value];
    }
    return newString;
}

/**
 Helper method to parse disconnection reason and returns appropriate message
 */
const char* disconnectionReasonToString(NclDisconnectionReason reason){
    switch(reason){
        case NCL_DISCONNECTION_LOCAL:
            return DISCONNECTION_LOCAL;
        case NCL_DISCONNECTION_TIMEOUT:
            return DISCONNECTION_TIMEOUT;
        case NCL_DISCONNECTION_FAILURE:
            return DISCONNECTION_FAILURE;
        case NCL_DISCONNECTION_REMOTE:
            return DISCONNECTION_REMOTE;
        case NCL_DISCONNECTION_CONNECTION_TIMEOUT:
            return DISCONNECTION_CONNECTION_TIMEOUT;
        case NCL_DISCONNECTION_LL_RESPONSE_TIMEOUT:
            return DISCONNECTION_RESPONSE_TIMEOUT;
        case NCL_DISCONNECTION_OTHER:
            return DISCONNECTION_OTHERS;
        default: break;
    }
    return DISCONNECTION_INVALID;
}

/**
 Call back method that will be provided as input to NCL library during initialization.
 All the call back events related to initilization, discovery, agree, provision, find and validate will be using this as call back to inform the status of the actions that are sent to NCL library.
 @param event NclEvent with different status updates related to that event
 @param userData Userinfo sent to NCL library during NCL library initialization
 */
void callback(NclEvent event, void* userData){
    
    NCLWrapper* wrapperObj= (__bridge NCLWrapper*)userData;

    @try {
        switch(event.type) {
                
            case NCL_EVENT_INIT:
                [wrapperObj updateNCLInitStatus:event.init.success];
                break;
                
            case NCL_EVENT_DISCOVERY:
                [wrapperObj onNymiDiscovery:event.discovery.nymiHandle];
                break;
                
            case NCL_EVENT_AGREEMENT: {
                NSString *agreecode = [NSString stringWithFormat:
                                       @"%d%d%d%d%d %d%d%d%d%d",
                                       event.agreement.leds[0][0],
                                       event.agreement.leds[0][1],
                                       event.agreement.leds[0][2],
                                       event.agreement.leds[0][3],
                                       event.agreement.leds[0][4],
                                       event.agreement.leds[1][0],
                                       event.agreement.leds[1][1],
                                       event.agreement.leds[1][2],
                                       event.agreement.leds[1][3],
                                       event.agreement.leds[1][4]
                                       ];
                [wrapperObj doNymiAgreeWithCode:agreecode withHandler:event.agreement.nymiHandle];
                break;
            }
                
            case NCL_EVENT_PROVISION:{
                
                DLog(@"KEYYYY %s",event.provision.provision.key);
                DLog(@"IDDDD %s",event.provision.provision.id);
                NSString *provisionKey = provisionIdToString(event.provision.provision.key);
                NSString *provisionID = provisionIdToString(event.provision.provision.id);
                DLog(@"in string KEYYYY %@",provisionKey);
                DLog(@"in string IDDDD %@",provisionID);
                
                [wrapperObj nymiProvisionedWithKey:provisionKey id:provisionID withProvision:event.provision.nymiHandle];
                break;
            }
                
            case NCL_EVENT_FIND:
                [wrapperObj nymiFoundHandle:event.find.nymiHandle];
                break;
                
            case NCL_EVENT_VALIDATION:
                [wrapperObj nymiValidated:event.validation.nymiHandle];
                break;
                
            case NCL_EVENT_DISCONNECTION:
                [wrapperObj nymiDisconnected:event.disconnection.nymiHandle reason:event.disconnection.reason];
                break;
                
            default: break;
        }
    }
    @catch (NSException *exception) {
        NSLog(@"%@", [NSString stringWithFormat:@"Exception %@", exception.reason]);
    }
    @finally {
    }
}

@end
