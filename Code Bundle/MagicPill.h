// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MagicPill.generated.h"

UCLASS()
class UNREALSCRIPTINGBASIC_API AMagicPill : public AActor
{
	GENERATED_BODY()
	
	// This is the trigger source (our pill spawner volume) #sec.13
	class APillSpawner* TriggerEventSource;

public:	
	// Sets default values for this actor's properties
	AMagicPill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// This returns the pill effect #sec2.2
	float getEffect();

	// My added items to the class
protected:
	// The amount of health that the pill has (+/-)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="values")
	float PillEffect;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* PillMesh;

	// This function is responsible for randomizing pill effects and is bound to the triggered event
	UFUNCTION()
	void OnTriggerEvent();

	
};



/*

UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Components")
UStaticMeshComponent* PillMesh;



// My Added Properties


protected:

// Pill Effect in Terms of Health (+/-)



*/