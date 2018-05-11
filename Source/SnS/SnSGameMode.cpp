// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SnSGameMode.h"
#include "SnSPawn.h"

ASnSGameMode::ASnSGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ASnSPawn::StaticClass();
}
