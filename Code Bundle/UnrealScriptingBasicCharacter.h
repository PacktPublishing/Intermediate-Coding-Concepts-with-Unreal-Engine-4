// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MyInterface.h"
#include "UnrealScriptingBasicCharacter.generated.h"

UCLASS(config=Game)
class AUnrealScriptingBasicCharacter : public ACharacter, public IMyInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AUnrealScriptingBasicCharacter();

	// Declare a function that must be implemented in c++ #sec3.3
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces")
	bool ReactToPlayerEntered();
	virtual bool ReactToPlayerEntered_Implementation() override;


	// Declare the override of the interface #sec3.1
	FString GetTestName() override;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// The variable containing the number of jumps
	int HowManyJumps;

	// Setting Up Particle System #sec1.2
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Effects)
	UParticleSystemComponent* VisualEFX;

	//Declare Functions to be bound to the delegates  #sec1.2
	void makeEFXVisible();
	void makeEFXInvisible();

	// Overrides BeginPlay #sec1.2
	virtual void BeginPlay() override;


	// Overrides BeginPlay #sec1.2
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Boolean variable to hold the mode (pickup on/off)
	bool PickupMode;

	// This function is to pick up objects
	void PickObjects();

	// Collision functionality setup #sec2.2
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
	void OnOverlapBegin(UPrimitiveComponent* Comp, AActor* otherActor, UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;

	float HP;	// Health of player

	UTextRenderComponent* HPText;





protected:

	
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// This overrides the Jump function of base character
	virtual void Jump() override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface




public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

class MyClass {
public:
	int values;
};













/*


virtual void Jump() override;


*/

/*
// Declare a Custom Event #sec1.3
DECLARE_EVENT(APillSpawner, FPlayerEntered)
FPlayerEntered OnPlayerEntered;


*/