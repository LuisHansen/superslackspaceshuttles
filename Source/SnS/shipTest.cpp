// Fill out your copyright notice in the Description page of Project Settings.

#include "shipTest.h"
#include "Camera/CameraComponent.h"
#include "Engine.h"


// Sets default values
AshipTest::AshipTest()
{
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

void AshipTest::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AshipTest::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AshipTest::Rotate_XAxis(float AxisValue) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, AxisValue);
}

void AshipTest::Rotate_YAxis(float AxisValue) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, AxisValue);
}

void AshipTest::StartGrowing()
{
	bGrowing = true;
}

void AshipTest::StopGrowing()
{
	bGrowing = false;
}

// Called when the game starts or when spawned
void AshipTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AshipTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
		if (bGrowing)
		{
			// Grow to double size over the course of one second
			CurrentScale += DeltaTime;
		}
		else
		{
			// Shrink half as fast as we grow
			CurrentScale -= (DeltaTime * 0.5f);
		}
		// Make sure we never drop below our starting size, or increase past double size.
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
	}

	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!CurrentVelocity.IsZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocation);
			
			/*FRotator ActorRotation = this->GetActorRotation();
			ActorRotation.Pitch += 10.0f;
			this->SetActorRotation(ActorRotation);*/

		}
	}
}

// Called to bind functionality to input
void AshipTest::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	// Respond when our "Grow" key is pressed or released.
	InputComponent->BindAction("Grow", IE_Pressed, this, &AshipTest::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AshipTest::StopGrowing);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &AshipTest::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AshipTest::Move_YAxis);

	// PNMDSC
	InputComponent->BindAxis("RotX", this, &AshipTest::Rotate_XAxis);
	InputComponent->BindAxis("RotY", this, &AshipTest::Rotate_YAxis);
}

