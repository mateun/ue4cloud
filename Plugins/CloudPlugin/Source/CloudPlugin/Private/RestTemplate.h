//
//  RestTemplate.h
//  MyCloudApiProject2
//
//  Created by Martin Gruscher on 24/01/16.
//  Copyright © 2016 Epic Games, Inc. All rights reserved.
//

#ifndef RestTemplate_h
#define RestTemplate_h

#ifdef _WIN32
#include "AllowWindowsPlatformTypes.h"
#endif
#include "curl/curl.h"
#ifdef _WIN32
#include "HideWindowsPlatformTypes.h"
#endif

// This struct is used internally
// to hold the incoming response
// data.
struct MemoryStruct {
    char *memory;
    size_t size;
};

// This struct is used internally
// to hold outgoing post data.
struct WriteThis {
    const char *readptr;
    long sizeleft;
};

// This function is the callback for incoming response data.
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

// This function is the callback when posting outgoing data and
// transmitting it byte by byte.
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp);



/**
    This class provides convenient functions
    to communicate with Http REST endpoints.
 */
class RestTemplate
{
private:
    CURL* _curlHandler;
    struct MemoryStruct _chunk;
    
public:
    RestTemplate();
    ~RestTemplate();
    
    // Issues an HTTP GET request towards the given
    // url and returns the raw response as a string.
    FString GetForString(FString restUrl);
    
    // Issues an HTTP POST request towards the given
    // url and returns the raw response as a string.
    FString PostForString(FString restUrl, FString body);
    
    
};

#endif /* RestTemplate_h */
