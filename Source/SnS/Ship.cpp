// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Camera/CameraComponent.h"
#include "Engine.h"


// Sets default values
AShip::AShip() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Create a camera and a visible object
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);
}

void AShip::Thrust(float intensity) {
	if (isWarping) return;

	float accelerationIntensity = FMath::Clamp(intensity, -1.0f, 1.0f);

	// Move at 100 units per second forward or backward
	if (this->isStopped()) {
		CurrentVelocity.X = 0.2f * intensity;
	} else {
		int directionCoefficient = this->isGoingForward() ? 1.0f : -1.0f;
		CurrentVelocity.X *= FMath::Pow(1.15f, directionCoefficient * accelerationIntensity);
	}
	
	if (isStopped()) {
		CurrentVelocity.X = 0.0f;
	}
	CurrentVelocity.X = FMath::Clamp<float>(CurrentVelocity.X, -MAX_VELOCITY, MAX_VELOCITY);
}

void AShip::RotateShip(float magnitude) {
	// Move at 100 units per second right or left
	CurrentRotation.Yaw += 10.f * magnitude;
}

void AShip::ChangeRoll(float AxisValue) {
	CurrentRotation.Roll += 10.f * AxisValue;
}

void AShip::ChangePitch(float AxisValue) {
	CurrentRotation.Pitch += 10.f * AxisValue;
}

void AShip::SpaceBarPressed() {
	this->warp();
}

void AShip::SpaceBarReleased() {
	this->stopWarp();
}

void AShip::warp() {
	isWarping = true;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "WRAPPING");
	CurrentVelocity.X = WARP_SPEED;
}

void AShip::stopWarp() {
	isWarping = false;
	CurrentVelocity.X = MAX_VELOCITY;
}

FVector AShip::GetProjectedVelocity() {
	return this->GetActorForwardVector() * CurrentVelocity.X;
}

bool AShip::isStopped() {
	return FMath::Abs(CurrentVelocity.X) < 0.2f;
}

bool AShip::isGoingForward() {
	return CurrentVelocity.X > 0;
}

// Called when the game starts or when spawned
void AShip::BeginPlay() {
	Super::BeginPlay();
	CurrentVelocity.X = 0.0F;

}

// Called every frame
void AShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
	}

	if (!isStopped()) {
		this->AddActorLocalRotation(this->CurrentRotation);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, CurrentVelocity.ToString());
	SetActorLocation(GetActorLocation() + this->GetProjectedVelocity());
	this->CurrentRotation = FRotator(0.f, 0.f, 0.f);
	/*
	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!CurrentVelocity.IsZero()) {
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocation);

			FRotator ActorRotation = this->GetActorRotation();
			ActorRotation.Pitch += 10.0f;
			this->SetActorRotation(ActorRotation);

		}
	}
	*/
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	// Respond when our "Grow" key is pressed or released.
	InputComponent->BindAction("SpaceBar", IE_Pressed, this, &AShip::SpaceBarPressed);
	InputComponent->BindAction("SpaceBar", IE_Released, this, &AShip::SpaceBarReleased);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("ArrowForward", this, &AShip::Thrust);
	InputComponent->BindAxis("ArrowSideways", this, &AShip::ChangeRoll);

	// PNMDSC
	InputComponent->BindAxis("MouseX", this, &AShip::RotateShip);
	InputComponent->BindAxis("MouseY", this, &AShip::ChangePitch);
}

