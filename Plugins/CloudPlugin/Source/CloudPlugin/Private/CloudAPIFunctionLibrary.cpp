//
//  CloudAPIFunctionLibrary.cpp
//  MyCloudApiProject2
//
//  Created by Martin Gruscher on 23/01/16.
//  Copyright © 2016 Epic Games, Inc. All rights reserved.
//

#include "CloudPluginPrivatePCH.h"
#include "CloudAPIFunctionLibrary.h"
#include "RestTemplate.h"
#include "JsonConverter.h"


UCloudAPIFunctionLibrary::UCloudAPIFunctionLibrary(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer) {
    UCloudAPIFunctionLibrary::StaticClass();
}

/**
 * Blueprintcallable function to create a new instance of this class and Initialize it already with the given settings.
 */
UCloudAPIFunctionLibrary* UCloudAPIFunctionLibrary::Initialize(UObject* WorldContextObject, TSubclassOf<UObject> UC, FString user, FString password) {
    
    UE_LOG(LogAPIFuncLibPlugin, Error, TEXT("Within public initialization function."));
    
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
    UCloudAPIFunctionLibrary* tempObject = NewObject<UCloudAPIFunctionLibrary>(UC->StaticClass());
    
    tempObject->Initialize(user, password);
    
    return tempObject;
}


void UCloudAPIFunctionLibrary::Initialize(FString user, FString password) {
    UE_LOG(LogAPIFuncLibPlugin, Verbose, TEXT("Within internal initialization function."));

}


FString UCloudAPIFunctionLibrary::GetRawJsonDataFromCollection(FString collectionName)
{
    // TODO: implement with concatenation of endpoint + collectionName
    // For now, we just test hardcoded
    RestTemplate restTemplate;
    FString url = "www.indygoof.com:8080/persist/foobar_collection";
    FString result = restTemplate.GetForString(url);
    return result;
}

FString UCloudAPIFunctionLibrary::SaveGameToCloud(USaveGame *SaveGameObject, const FString& SaveKey)
{
    RestTemplate restTemplate;
    FString url = "www.indygoof.com:8080/persist/foobar_collection";
    FString body = JsonConverter::ConvertUObjectToJsonString(SaveGameObject);
    FString result = restTemplate.PostForString(url, body);
    return result;
}

void UCloudAPIFunctionLibrary::Shutdown() {
    
    
}
