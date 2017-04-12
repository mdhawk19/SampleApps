//
//  TestFunctions.hpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-08-11.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef TestFunctions_hpp
#define TestFunctions_hpp

class NymiApi;

struct resultPack{
    
    bool napiResponse = false;
    bool opSuccess = false;
    std::condition_variable cv;
    std::mutex mtx;
};

bool provisionBand(NymiApi * napi);
bool unprovisionBand();
bool getRandom();
bool getSymmetric();
bool getTotp();
bool sign();
bool notify();

#endif /* TestFunctions_hpp */
