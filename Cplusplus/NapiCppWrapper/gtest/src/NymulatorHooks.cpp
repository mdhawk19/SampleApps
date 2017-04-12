//
//  NymulatorHooks.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-08-12.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include <iostream>
#include <thread>
#include <cassert>
#include "NymulatorHooks.h"
#include <gobs/dyad/dyad.h>
extern "C" {
    extern void nclLock();
    extern void nclUnlock();
}

/* ----------------------
 dyad related stuff
 -----------------------*/
const int ndaPort = 9087;

static dyad_Stream* ndaStream=NULL;
static int ndaReady=0;

void atEvent(dyad_Event* event){
    
    switch(event->type){
        case DYAD_EVENT_READY: ndaReady=1; break;
        case DYAD_EVENT_DATA: break;
        case DYAD_EVENT_TICK: break;
        case DYAD_EVENT_ERROR:
        case DYAD_EVENT_CLOSE:
        case DYAD_EVENT_DESTROY:
            if(event->stream==ndaStream){
                std::cerr<<"nda dyad event: "<<event->type<<"\n";
                break;
            }
            //fall through
        default:
            std::cerr<<"unhandled dyad event: "<<event->type<<"\n";
            break;
    }
}

void addListeners(dyad_Stream* stream){
    unsigned i;
    for(i=DYAD_EVENT_NULL; i<=DYAD_EVENT_TICK; ++i){
        dyad_addListener(stream, i, atEvent, NULL);
    }
}

void initDyad(){
    
    nclLock();
    ndaStream=dyad_newStream();
    addListeners(ndaStream);
    dyad_connect(ndaStream, "127.0.0.1", ndaPort);
    nclUnlock();
    {
        unsigned i;
        for(i=0; i<5000&&!ndaReady; ++i) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        assert(ndaReady);
    }
}

/* ----------------------
 Nymulator hooks
 -----------------------*/

const int waitMs = 500; //allow Nymulator some time to recieve and act upon the cmd sent by dyad

void nymuAddBand(){
    
    nclLock();
    dyad_write(ndaStream, "a testBand", 11);
    std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    nclUnlock();
}

void nymuProvisionBand(){
    
    nclLock();
    dyad_write(ndaStream, "p testBand", 11);
    std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    nclUnlock();
}

void nymuRemoveBand(){
    
    nclLock();
    dyad_write(ndaStream, "r testBand", 11);
    std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    nclUnlock();
}

void nymuUnclaspBand(){
    
    nclLock();
    dyad_write(ndaStream, "u testBand", 11);
    std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    nclUnlock();
}

void nymuClaspBand(){
    
    nclLock();
    dyad_write(ndaStream, "c testBand", 11);
    std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    nclUnlock();
}
