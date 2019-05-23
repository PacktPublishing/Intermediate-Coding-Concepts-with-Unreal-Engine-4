// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealScriptingBasic.h"
#include "MyInterface.h"


// This function does not need to be modified.
UMyInterface::UMyInterface(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IMyInterface functions that are not pure virtual.

FString IMyInterface::GetTestName() {
	unimplemented();
	return FString();
}



