//
//  JsonConverter.cpp
//  MyCloudApiProject2
//
//  Created by Martin Gruscher on 24/01/16.
//  Copyright © 2016 Epic Games, Inc. All rights reserved.
//

#include "CloudPluginPrivatePCH.h"
#include "JsonConverter.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

// Dummy test to see if everything works
// basically
FString DummyTest() {
    
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"game\":\"mygame\",\"maxPlayers\":4}";
    rapidjson::Document d;
    d.Parse(json);
    
    // 2. Modify it by DOM.
    rapidjson::Value& s = d["maxPlayers"];
    s.SetInt(s.GetInt() + 1);
    
    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    
    // Output {"project":"rapidjson","maxPlayers":5}
    std::cout << buffer.GetString() << std::endl;
    return buffer.GetString();
    

}

FString JsonConverter::ConvertUObjectToJsonString(UObject* theObject)
{
    
    // return DummyTest();
    
    // First, create a map of all properties in our object
    TMap<FString, UProperty*> properties;
    for (TFieldIterator<UProperty> it(theObject->GetClass(), EFieldIteratorFlags::SuperClassFlags::IncludeSuper); it; ++it) {
        properties.Add(*it->GetNameCPP(), *it);
    }
    
    // Test all of the types we could possibly have.
    UInt64Property *int64prop = NULL;
    UIntProperty *int32prop = NULL;
    UInt16Property *int16prop = NULL;
    UInt8Property *int8prop = NULL;
    UBoolProperty *boolProp = NULL;
    UDoubleProperty *doubleProp = NULL;
    UFloatProperty *floatProp = NULL;
    UStrProperty *strProp = NULL;
    UNameProperty *nameProp = NULL;
    UTextProperty *textProp = NULL;
    UStructProperty *structProp = NULL;
    
    // Create a basic json object
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    
    writer.StartObject();
    
    // Go through all properties
    // and write the appropriate
    // json elements, based
    // on the type of the property.
    for (auto& Elem : properties)
    {
        // Write the property name as the json element name
        writer.String(TCHAR_TO_ANSI(*Elem.Key));
        
        // Write the value based on the property type
        if ((int64prop = Cast<UInt64Property>(Elem.Value)) != NULL) {
            FPlatformMisc::LocalPrint(
                *FString::Printf(
                    TEXT("(%s => int64)\n"),
                    *Elem.Key)
            );
        } else if ((int32prop = Cast<UIntProperty>(Elem.Value)) != NULL) {
            int val = int32prop->GetOptionalPropertyValue_InContainer(theObject);
            FPlatformMisc::LocalPrint(
                *FString::Printf(TEXT("(%s => int32: %d)\n"),
                                 *Elem.Key,
                                 val));
            writer.Int(val);

        } else if ((int16prop = Cast<UInt16Property>(Elem.Value)) != NULL) {
            unsigned int val = int16prop->GetOptionalPropertyValue_InContainer(theObject);
            FPlatformMisc::LocalPrint(
                *FString::Printf(TEXT("(%s => int16)\n"),
                                 *Elem.Key));
            writer.Int(val);
        } else if ((int8prop = Cast<UInt8Property>(Elem.Value)) != NULL) {
            FPlatformMisc::LocalPrint(
                *FString::Printf(TEXT("(%s => int8)\n"),
                                 *Elem.Key));
        } else if ((strProp = Cast<UStrProperty>(Elem.Value)) != NULL) {
            FString val = strProp->GetOptionalPropertyValue_InContainer(theObject);
            FPlatformMisc::LocalPrint(
                *FString::Printf(TEXT("(%s => string: %s)\n"),
                                 *Elem.Key,
                                 *val));
            writer.String(TCHAR_TO_ANSI(*val));
        }
    }
    // End this json object
    writer.EndObject();
    
    /*writer.String("hello");
    writer.String("world");
    writer.String("t");
    writer.Bool(true);
    writer.String("f");
    writer.Bool(false);
    writer.String("n");
    writer.Null();
    writer.String("i");
    writer.Uint(123);
    writer.String("pi");
    writer.Double(3.1416);
    writer.String("a");
    writer.StartArray();
    for (unsigned i = 0; i < 4; i++)
        writer.Uint(i);
    writer.EndArray();
    writer.EndObject();*/
    
    std::cout << "Json save game object: " << s.GetString() << std::endl;
    
    return s.GetString();
    
}
