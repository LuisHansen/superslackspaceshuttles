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

void AShip::Move_XAxis(float AxisValue) {
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AShip::Move_YAxis(float AxisValue) {
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AShip::ChangeRoll(float AxisValue) {
	CurrentRotation.Roll += 10.f * AxisValue;
}

void AShip::ChangePitch(float AxisValue) {
	CurrentRotation.Pitch += 10.f * AxisValue;
}

void AShip::StartGrowing() {
	bGrowing = true;
}

void AShip::StopGrowing() {
	bGrowing = false;
}

// Called when the game starts or when spawned
void AShip::BeginPlay() {
	Super::BeginPlay();
	CurrentVelocity.X = 100.0F;

}

// Called every frame
void AShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
	}

	GEngine->AddOnScreenDebugMessage(-2, 1.5, FColor::Red , "ROTATION");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->CurrentRotation.ToString());
	this->AddActorLocalRotation(this->CurrentRotation);
	SetActorLocation(GetActorLocation() + this->GetActorForwardVector());
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
	InputComponent->BindAction("Grow", IE_Pressed, this, &AShip::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AShip::StopGrowing);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("ArrowForward", this, &AShip::Move_XAxis);
	InputComponent->BindAxis("ArrowSideways", this, &AShip::ChangeRoll);

	// PNMDSC
	InputComponent->BindAxis("MouseX", this, &AShip::Move_YAxis);
	InputComponent->BindAxis("MouseY", this, &AShip::ChangePitch);
}

