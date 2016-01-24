//
//  JsonConverter.h
//  MyCloudApiProject2
//
//  Created by Martin Gruscher on 24/01/16.
//  Copyright © 2016 Epic Games, Inc. All rights reserved.
//

#ifndef JsonConverter_h
#define JsonConverter_h

/**
    This class handles the conversion from UObjects to Json Strings
    and vice versa.
 */
class JsonConverter {
    
public:
    static FString ConvertUObjectToJsonString(UObject* theObject);
};


#endif /* JsonConverter_h */
