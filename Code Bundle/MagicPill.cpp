// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealScriptingBasic.h"
#include "MagicPill.h"
#include "UnrealScriptingBasicCharacter.h"
#include "PillSpawner.h"

// Sets default values
AMagicPill::AMagicPill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PillEffect = 0.0f;

	PillMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));

	if (MeshAsset.Object != nullptr) {
		PillMesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AMagicPill::BeginPlay()
{
	Super::BeginPlay();
	PillMesh->SetSimulatePhysics(true);

	// setting up the trigger event source for this object #sec1.3
	auto Itr = TActorIterator<APillSpawner>(GetWorld());
	TriggerEventSource = *Itr;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%s is assigned to me!!!"),
		*(TriggerEventSource->GetName())));

	if (TriggerEventSource != nullptr) {
		TriggerEventSource->OnPlayerEntered.AddUObject(this, &AMagicPill::OnTriggerEvent);
	}


}

// Called every frame
void AMagicPill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicPill::OnTriggerEvent() {
	PillEffect = FMath::RandRange(-150.f, 150.f);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green,
		FString::Printf(TEXT("%f is my new value!!!"), PillEffect));

}

// This returns the pill effect #sec2.2
float AMagicPill::getEffect() {
	return PillEffect;
}