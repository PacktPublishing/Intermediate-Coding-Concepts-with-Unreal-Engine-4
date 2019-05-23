// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PillSpawner.generated.h"




UCLASS()
class UNREALSCRIPTINGBASIC_API APillSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillSpawner();

	// Declare the custom event signature for pill spawner #sec1.3
	DECLARE_EVENT(APillSpawner, FPlayerEntered)

	// Declare the event using the above signature #sec1.3
	FPlayerEntered OnPlayerEntered;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// This is the Item to Spawn
	UPROPERTY(EditAnyWhere, Category="Spawning")
	TSubclassOf<class AMagicPill> ItemToSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category="Spawning")
	FVector GetRandomPointInVolume();

private:
	// This is the Spawning Volume as a Box Component
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category="Spawning", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* SpawningVolume;

	// This function spawns magic pills
	void SpawnPills();

	// Event associated with another actor entering our volume
	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	// Event associated with another actor exiting our volume
	virtual void NotifyActorEndOverlap(AActor* otherActor) override;
};




/*

UPROPERTY(EditAnyWhere, Category="Spawning")
TSubclassOf<class AMagicPill> ItemToSpawn;

UFUNCTION(BlueprintPure, Category="Spawning")
FVector GetRandomPointInVolume();


private:


UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category="Spawning", meta=(AllowPrivateAccess="true"))
class UBoxComponent* SpawningVolume;

// This function spawns magic pills
void SpawnPills();

*/
/*
// Declare a Custom Event #sec1.3
DECLARE_EVENT(APillSpawner, FPlayerEntered)
FPlayerEntered OnPlayerEntered;

*/