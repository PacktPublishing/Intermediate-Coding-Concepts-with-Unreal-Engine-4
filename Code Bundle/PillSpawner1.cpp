// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealScriptingBasic.h"
#include "PillSpawner1.h"


// Sets default values
APillSpawner1::APillSpawner1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APillSpawner1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APillSpawner1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

