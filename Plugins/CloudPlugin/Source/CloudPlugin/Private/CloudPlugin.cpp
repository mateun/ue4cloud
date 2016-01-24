// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "CloudPluginPrivatePCH.h"


DEFINE_LOG_CATEGORY(LogAPIFuncLibPlugin);

#define LOCTEXT_NAMESPACE "FCloudPluginModule"

void FCloudPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    UE_LOG(LogAPIFuncLibPlugin, Verbose, TEXT("In Starting the Cloud API plugin module"));
}

void FCloudPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCloudPluginModule, CloudPlugin)