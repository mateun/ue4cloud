//
//  CloudAPIFunctionLibrary.h
//  MyCloudApiProject2
//
//  Created by Martin Gruscher on 23/01/16.
//  Copyright © 2016 Epic Games, Inc. All rights reserved.
//



#include "CloudAPIFunctionLibrary.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UCloudAPIFunctionLibrary : public UObject {
    
    GENERATED_UCLASS_BODY()
    
private:
    // Internal initialization stuff
    void Initialize(FString user, FString password);
    
public:
    UFUNCTION(BlueprintCallable, Category="CloudAPI")
    void Shutdown();
    
    /**
     * The actual initializing function; Returns a new instance of itself and initializes stuff like EnvironmentHandles, etc
     *
     **/
    UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Create and initialize Object From Blueprint", CompactNodeTitle = "Init", Keywords = "new create blueprint"), Category = CloudAPI)
    static UCloudAPIFunctionLibrary* Initialize(UObject* WorldContextObject, TSubclassOf<UObject> UC, FString user, FString password);
    
    UFUNCTION(BlueprintCallable, Category = "CloudAPI", meta = (DisplayName = "GetRawJsonDataFromCollection", Keywords = "cloud api cloudapi json raw fstring call"))
    FString GetRawJsonDataFromCollection(FString collectionName);

    
    UFUNCTION(BlueprintCallable, Category="CloudAPI")
    FString SaveGameToCloud(USaveGame *SaveGameObject, const FString& SaveKey);
    
};

