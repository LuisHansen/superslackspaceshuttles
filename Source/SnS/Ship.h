// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

UCLASS()
class SNS_API AShip : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();
	//Input functions
	void Move_XAxis(float AxisValue);
	void RotateShip(float magnitude);
	void Thrust(float intensity);
	void ChangePitch(float AxisValue);
	void ChangeRoll(float AxisValue);
	void SpaceBarPressed();
	void SpaceBarReleased();

	void warp();
	void stopWarp();
	FVector GetProjectedVelocity();
	bool isStopped();
	bool isGoingForward();

	//Input variables
	FVector CurrentVelocity;
	FRotator CurrentRotation;
	bool isWarping;

	// Constants
	int MAX_VELOCITY = 200.0f;
	float WARP_SPEED = 100000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;

};
