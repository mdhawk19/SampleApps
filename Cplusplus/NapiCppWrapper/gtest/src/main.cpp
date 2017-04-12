//
//  main.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-05-06.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include <iostream>
#include <chrono>
#include "gtest/gtest.h"
#include "TestsProvision.h"
#include "TestsNotifications.h"
#include "NymulatorHooks.h"
#include "NymiApi.h"
#include "NymiProvision.h"

static NymiApi * napi;

/* -----------------------------------------------------------------------------
 set up Napi and Nymulator, get a provisioned band, tear down Napi and Nymulator
 -------------------------------------------------------------------------------*/

const int muxPort = 9089;

bool initNapi(){
    
    errorCallback onError = [](napiError nErr) {
        std::cout << "NAPI ERROR: "<< nErr.errorString << std::endl;
    };
    
    try {
        //init Napi
        nymi::ConfigOutcome initResult;
        napi = NymiApi::getNymiApi(initResult,onError, ".", nymi::LogLevel::debug, muxPort, "127.0.0.1");
        
        if (initResult != nymi::ConfigOutcome::okay) {
            std::cout << "~~~~NYMI API INITIALIZATION FAILED, reason: "<<initResult<<std::endl;
            return false;
        }
        
        //init Dyad
        initDyad();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "NymiApi initialization succeeded\n";
        
        return true;
        
    }
    catch (...){
        std::cout<<"~~~~NYMI API INITIALIZATION FAILED. In initNapi() catch.\n";
        return 1;
    }
}

bool finishNapi(){
    
    delete napi;
    return true;
}

/* --------------------------------------------------------------------------------------
 google test code that uses the functions above to test various parts and pieces of Napi
 ---------------------------------------------------------------------------------------*/
class NapiTest : public ::testing::Test {

protected:
    static void SetUpTestCase() {
        if (!initNapi()){
            std::cerr<<"~~~~EXITING\n";
            exit(1);
        }
        if (!provisionBand(napi)){
            std::cerr<<"~~~~PROVISIONING BAND FAILED. EXITING\n";
            exit(1);
        }
    }
    static void TearDownTestCase() {
        ASSERT_TRUE(unprovisionBand());
        nymuRemoveBand();
        finishNapi();
    }
};

TEST_F (NapiTest, Random){ ASSERT_TRUE(getRandom()); }
TEST_F (NapiTest, SymmetricKey){ ASSERT_TRUE(getSymmetric()); }
TEST_F (NapiTest, Totp){ ASSERT_TRUE(getTotp()); }
TEST_F (NapiTest, Sign){ ASSERT_TRUE(sign()); }
TEST_F (NapiTest, Notify){ ASSERT_TRUE(notify()); }
TEST_F (NapiTest, FoundChange){ ASSERT_TRUE(notiOnFoundChange(napi)); }
TEST_F (NapiTest, PresenceChange){ ASSERT_TRUE(notiOnPresenceChange(napi)); }
TEST_F (NapiTest, NotificationStatus){ ASSERT_TRUE(notificationGet(napi)); }


/* ----------------------
    run google tests
 -----------------------*/
int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


/*
 Using Nymu CLI

 cd nymulator/src/NymuFrontEndCli
 cmake .
 cmake --build .
 ./NymuCli 9089 9087 where muxPort is 9089 and ndaPort is 9087
 
 also helpful:
 ./NymuCli -h
*/
