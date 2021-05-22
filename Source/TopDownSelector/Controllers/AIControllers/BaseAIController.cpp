// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"

void ABaseAIController::MoveCharacterTo(const FVector Coordinates)
{
	APawn* const CurrentPawn = GetPawn();
	if (CurrentPawn == nullptr)
	{
		return;
	}

	float const Distance = FVector::Dist(Coordinates, CurrentPawn->GetActorLocation());
	// We need to issue move command only if far enough in order for walk animation to play correctly
	if (Distance < 120.0f)
	{
		return;
	}
	MoveToLocation(Coordinates);
}