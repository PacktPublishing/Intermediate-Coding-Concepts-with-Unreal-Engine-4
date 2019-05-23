// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealScriptingBasic.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "UnrealScriptingBasicGameMode.h"
#include "UnrealScriptingBasicCharacter.h"
#include "MagicPill.h"


//////////////////////////////////////////////////////////////////////////
// AUnrealScriptingBasicCharacter

// Implementation of the React To Player Entered event #sec3.3
bool AUnrealScriptingBasicCharacter::ReactToPlayerEntered_Implementation() {
	HP -= 100;
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f "), HP)));
	return true;
}


FString AUnrealScriptingBasicCharacter::GetTestName() {
	FString myName = this->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, 
		FString::Printf(TEXT("%s Implements the Interface."),*myName));

	return myName;
}


AUnrealScriptingBasicCharacter::AUnrealScriptingBasicCharacter()
{
	
	// Player's initial HP
	HP = 750.f;



	HowManyJumps = 0;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	// Setting Up the Particle System #sec1.2
	VisualEFX = CreateDefaultSubobject<UParticleSystemComponent>("VisEFX");
	VisualEFX->AttachTo(RootComponent);
	auto ParticleSystem = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Sparks.P_Sparks'"));
	
	if (ParticleSystem.Object != nullptr) {
		VisualEFX->SetTemplate(ParticleSystem.Object);
		VisualEFX->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	}
	VisualEFX->SetVisibility(false);

	// At start pickup is off #sec2.1
	PickupMode = false;

	HPText = CreateDefaultSubobject<UTextRenderComponent>("My_HP");
	HPText->AttachTo(RootComponent);
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f"), HP)));

}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealScriptingBasicCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnrealScriptingBasicCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnrealScriptingBasicCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnrealScriptingBasicCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnrealScriptingBasicCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnrealScriptingBasicCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnrealScriptingBasicCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnrealScriptingBasicCharacter::OnResetVR);

	PlayerInputComponent->BindAction("PickingUp", IE_Pressed, this, &AUnrealScriptingBasicCharacter::PickObjects);

}


void AUnrealScriptingBasicCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnrealScriptingBasicCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUnrealScriptingBasicCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUnrealScriptingBasicCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealScriptingBasicCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealScriptingBasicCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUnrealScriptingBasicCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUnrealScriptingBasicCharacter::Jump() {

	Super::Jump();
	UE_LOG(LogTemp, Warning, TEXT("Player is jumping!!!"))

	HowManyJumps++;
	FString JumpsText = FString::Printf(TEXT("The player has jumped %d times...."), HowManyJumps);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *JumpsText)


		int * JumpHeight;							// Declare a pointer variable for the Jump Height

	JumpHeight = new int(); // (int*)malloc(sizeof(int));		// Allocates the memory for this pointer
	*JumpHeight = 10;							// Assigns the content of this pointer

												// Here we log this value of Jump Height
	FString jumpheightText = FString::Printf(TEXT("The player can jump %d meters up in the air."), *JumpHeight);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *jumpheightText);

	delete JumpHeight;							// This deallocates the memory

	JumpHeight = 0;								// Set the pointer to null


	TSharedPtr<MyClass> emptySharedClass;  // This is an empty shared pointer
	TSharedPtr<MyClass> firstSharedObject(new MyClass());   // This declares the shared pointer

															// When you copy a shared pointer, ref. count increments
	TSharedPtr<MyClass> anotherObject = firstSharedObject;

	// You can get the reference count like this
	firstSharedObject.GetSharedReferenceCount();

	// You can dereference shared pointers like so
	firstSharedObject->values;
	firstSharedObject.Get()->values;
	(*firstSharedObject).values;

	// You can check to see if a shared pointer is valid
	if (firstSharedObject.IsValid()) {
		// do things with the shared pointer
	}

	// or like so
	if (firstSharedObject.Get() != NULL) {
		// do things with the shared pointer
	}

//	VisualEFX->SetVisibility(true);

}

void AUnrealScriptingBasicCharacter::BeginPlay() {
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World != nullptr) {
		AUnrealScriptingBasicGameMode* myGameMode =Cast<AUnrealScriptingBasicGameMode>(UGameplayStatics::GetGameMode(World));
		if (myGameMode != nullptr) {
			myGameMode->CharacterVisualEffectsDelegateStart.BindUObject(this, &AUnrealScriptingBasicCharacter::makeEFXVisible);
			myGameMode->CharacterVisualEffectsDelegateStop.BindUObject(this, &AUnrealScriptingBasicCharacter::makeEFXInvisible);
		}
	}
	HPText->SetRelativeLocation(FVector(0, -30, 75));
	HPText->SetRelativeRotation(FRotator(0, 180.f, 0));
	HPText->SetXScale(1.f);
	HPText->SetYScale(1.f);
	HPText->SetWorldSize(15);
	HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f "), HP)));

}

void AUnrealScriptingBasicCharacter::makeEFXVisible() {
	VisualEFX->SetVisibility(true);
}

void AUnrealScriptingBasicCharacter::makeEFXInvisible() {
	VisualEFX->SetVisibility(false);
}


void AUnrealScriptingBasicCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	UWorld* World = GetWorld();
	if (World != nullptr) {
		AUnrealScriptingBasicGameMode* myGameMode = Cast<AUnrealScriptingBasicGameMode>(UGameplayStatics::GetGameMode(World));
		if (myGameMode != nullptr) {
			myGameMode->CharacterVisualEffectsDelegateStart.Unbind();
			myGameMode->CharacterVisualEffectsDelegateStop.Unbind();
		}
	}

}

void AUnrealScriptingBasicCharacter::PickObjects() {
	PickupMode = !PickupMode;
	if (PickupMode) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Pick Up Mode Toggeled On!!!")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Pick Up Mode Toggeled Off!!!")));

	}

}


void AUnrealScriptingBasicCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	if (RootComponent) {
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AUnrealScriptingBasicCharacter::OnOverlapBegin);
	}

}

void AUnrealScriptingBasicCharacter::OnOverlapBegin_Implementation(UPrimitiveComponent* Comp,
	AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult) {
	
	AMagicPill* isPill= Cast<AMagicPill>(otherActor);

	if (isPill != nullptr) {
		if (PickupMode) {
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue,
				FString::Printf(TEXT("Colliding with %s!!!"), *(otherActor->GetName())));
			HP += isPill->getEffect();
			isPill->Destroy();
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green,
				FString::Printf(TEXT("My new healt: %f!!!"), HP));
			HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f "), HP)));
		}
	}
}


























