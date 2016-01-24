//
//  RestTemplate.cpp
//  MyCloudApiProject2
//
//  Created by Martin Gruscher on 24/01/16.
//  Copyright © 2016 Epic Games, Inc. All rights reserved.
//

#include "CloudPluginPrivatePCH.h"
#include "RestTemplate.h"
#include <stdlib.h>


static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct WriteThis *pooh = (struct WriteThis *)userp;
    
    if(size*nmemb < 1)
        return 0;
    
    if(pooh->sizeleft) {
        *(char *)ptr = pooh->readptr[0]; /* copy one single byte */
        pooh->readptr++;                 /* advance pointer */
        pooh->sizeleft--;                /* less data left */
        return 1;                        /* we return 1 byte at a time! */
    }
    
    return 0;                          /* no more data left to deliver */
}


RestTemplate::RestTemplate()
{
    _chunk.memory = (char*) malloc(1);
    _chunk.size = 0;
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* In windows, this will init the winsock stuff */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    /* Check for errors */
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed: %s\n",
                curl_easy_strerror(res));
        exit(1);
    }
    
    
    _curlHandler = curl_easy_init();
    
    
    
    
    
}

FString RestTemplate::GetForString(FString restUrl)
{
    CURLcode res;
    char curl_errbuf[CURL_ERROR_SIZE];
    int err;
    
    // FString is a wide string, but we need
    // to pass in single-byte strings
    // to libcurl for the URL.
    char* url = (char*)TCHAR_TO_ANSI(*restUrl);
    
    curl_easy_setopt(_curlHandler, CURLOPT_URL, url);
    curl_easy_setopt(_curlHandler, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(_curlHandler, CURLOPT_WRITEDATA, (void*) &_chunk);
    curl_easy_setopt(_curlHandler, CURLOPT_ERRORBUFFER, curl_errbuf);
    
    res = curl_easy_perform(_curlHandler);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s For url: %s\n",
                curl_easy_strerror(res),(char*) *restUrl);
        
    } else {
        printf("%lu bytes received\n", (long)_chunk.size);
        printf("content: %s\n", _chunk.memory);
        printf("curl setup ok\n");
    }

    return _chunk.memory;
}

FString RestTemplate::PostForString(FString restUrl, FString body)
{
    CURLcode res;
    
    struct WriteThis pooh;
    
    pooh.readptr = TCHAR_TO_ANSI(*body);
    pooh.sizeleft = (long)strlen(TCHAR_TO_ANSI(*body));
    
    if(_curlHandler) {
        
        // FString is a wide string, but we need
        // to pass in single-byte strings
        // to libcurl for the URL.
        char* url = (char*)TCHAR_TO_ANSI(*restUrl);
        
        /* First set the URL that is about to receive our POST. */
        curl_easy_setopt(_curlHandler, CURLOPT_URL, url);
        
        /* Now specify we want to POST data */
        curl_easy_setopt(_curlHandler, CURLOPT_POST, 1L);
        
        /* we want to use our own read function */
        curl_easy_setopt(_curlHandler, CURLOPT_READFUNCTION, read_callback);
        
        /* pointer to pass to our read function */
        curl_easy_setopt(_curlHandler, CURLOPT_READDATA, &pooh);
        
        /* get verbose debug output please */
        curl_easy_setopt(_curlHandler, CURLOPT_VERBOSE, 1L);
        
        /* set content-type application/json header */
        struct curl_slist *headers=NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(_curlHandler, CURLOPT_HTTPHEADER, headers);
        
        /*
         If you use POST to a HTTP 1.1 server, you can send data without knowing
         the size before starting the POST if you use chunked encoding. You
         enable this by adding a header like "Transfer-Encoding: chunked" with
         CURLOPT_HTTPHEADER. With HTTP 1.0 or without chunked transfer, you must
         specify the size in the request.
         */
#ifdef USE_CHUNKED
        {
            struct curl_slist *chunk = NULL;
            
            chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
            res = curl_easy_setopt(_curlHandler, CURLOPT_HTTPHEADER, chunk);
            /* use curl_slist_free_all() after the *perform() call to free this
             list again */
        }
#else
        /* Set the expected POST size. If you want to POST large amounts of data,
         consider CURLOPT_POSTFIELDSIZE_LARGE */
        curl_easy_setopt(_curlHandler, CURLOPT_POSTFIELDSIZE, pooh.sizeleft);
#endif
        
#ifdef DISABLE_EXPECT
        /*
         Using POST with HTTP 1.1 implies the use of a "Expect: 100-continue"
         header.  You can disable this header with CURLOPT_HTTPHEADER as usual.
         NOTE: if you want chunked transfer too, you need to combine these two
         since you can only set one list of headers with CURLOPT_HTTPHEADER. */
        
        /* A less good option would be to enforce HTTP 1.0, but that might also
         have other implications. */
        {
            struct curl_slist *chunk = NULL;
            
            chunk = curl_slist_append(chunk, "Expect:");
            res = curl_easy_setopt(_curlHandler, CURLOPT_HTTPHEADER, chunk);
            /* use curl_slist_free_all() after the *perform() call to free this
             list again */
        }
#endif
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(_curlHandler);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            return "POST 500 ERROR";
        } else {
            return "POST 200 OK";
        }
    }
    
    return "POST 500 ERROR";
}

RestTemplate::~RestTemplate()
{
    curl_easy_cleanup(_curlHandler);
    free(_chunk.memory);
    curl_global_cleanup();
}