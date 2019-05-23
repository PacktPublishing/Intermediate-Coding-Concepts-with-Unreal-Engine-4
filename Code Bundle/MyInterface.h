// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class UNREALSCRIPTINGBASIC_API IMyInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Set up the Interface functionality #sec3.1
	virtual FString GetTestName();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category= Interfaces)
	bool ReactToPlayerExited();

	// Declare a function that must be implemented in c++ #sec3.3
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interfaces)
	bool ReactToPlayerEntered();
	

	
};

