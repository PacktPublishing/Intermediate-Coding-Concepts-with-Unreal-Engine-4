// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealScriptingBasic.h"
#include "PillSpawner.h"
#include "MagicPill.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "UnrealScriptingBasicCharacter.h"
#include "UnrealScriptingBasicGameMode.h"
#include "MyInterface.h"



// Sets default values
APillSpawner::APillSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolumeBox"));
	RootComponent = SpawningVolume;

	// This establishes the item to spawn is a magic pill
	//ItemToSpawn = AMagicPill::StaticClass();
}

// Called when the game starts or when spawned
void APillSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnPills();

	//GetTestName();
	
}

// Called every frame
void APillSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// This is the function that returns a random point in the volume
FVector APillSpawner::GetRandomPointInVolume() {
	FVector Origin = SpawningVolume->Bounds.Origin;
	FVector Extent = SpawningVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

void APillSpawner::SpawnPills() {
	if (ItemToSpawn != NULL) {
		UWorld* const World = GetWorld();
		if (World) {
			FVector SpawnLocation = GetRandomPointInVolume();
			FRotator SpawnRotation;
			SpawnRotation.Pitch = FMath::FRand()*360.0f;
			SpawnRotation.Roll = FMath::FRand()*360.0f;
			SpawnRotation.Yaw = FMath::FRand()*360.0f;

			AMagicPill* SpawnedPill = World->SpawnActor<AMagicPill>(ItemToSpawn, SpawnLocation, SpawnRotation);
		}
	}
}

void APillSpawner::NotifyActorBeginOverlap(AActor* otherActor) {
	AUnrealScriptingBasicCharacter* isOtherPlayer = Cast<AUnrealScriptingBasicCharacter>(otherActor);

	if (isOtherPlayer) {
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s entered the volume!!!"), *(otherActor->GetName())));
		SpawnPills();


		// Execute Delegate to make player sparks visible #sec1.2
		UWorld* world = GetWorld();
		if (world != nullptr) {
			AUnrealScriptingBasicGameMode* gameMode = Cast<AUnrealScriptingBasicGameMode>(UGameplayStatics::GetGameMode(world));
			if (gameMode != nullptr) {
				gameMode->CharacterVisualEffectsDelegateStart.ExecuteIfBound();
			}
		}

		// Broadcast the player entered event so that the pills randomize #sec1.3
		OnPlayerEntered.Broadcast();

		for (TActorIterator<AActor> Itr(GetWorld(), AActor::StaticClass()); Itr; ++Itr) {
			AActor* Actor = *Itr;
			IMyInterface* MyInterfaceInstance = Cast<IMyInterface>(Actor);
			if (MyInterfaceInstance) {
				MyInterfaceInstance->Execute_ReactToPlayerEntered(Actor);
			}
		}

	}
}


void APillSpawner::NotifyActorEndOverlap(AActor * otherActor) {
	AUnrealScriptingBasicCharacter* isOtherPlayer = Cast<AUnrealScriptingBasicCharacter>(otherActor);
	if (isOtherPlayer) {
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s exited the volume!!!"), *(otherActor->GetName())));
		UWorld* world = GetWorld();
		if (world != nullptr) {
			AUnrealScriptingBasicGameMode* gameMode = Cast<AUnrealScriptingBasicGameMode>(UGameplayStatics::GetGameMode(world));
			if (gameMode != nullptr) {
				gameMode->CharacterVisualEffectsDelegateStop.ExecuteIfBound();
			}
		}
	}

}