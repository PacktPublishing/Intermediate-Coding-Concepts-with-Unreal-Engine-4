// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "UnrealScriptingBasicGameMode.generated.h"

//Declare the delegate #sec1.2
DECLARE_DELEGATE(FStandardDelegateSignature)

UCLASS(minimalapi)
class AUnrealScriptingBasicGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealScriptingBasicGameMode();

	FStandardDelegateSignature CharacterVisualEffectsDelegateStart;
	FStandardDelegateSignature CharacterVisualEffectsDelegateStop;
};



