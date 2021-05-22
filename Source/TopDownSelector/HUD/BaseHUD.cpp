// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseHUD.h"

#include "TopDownSelector/GameModes/GameModeMain.h"

void ABaseHUD::DrawHUD() 
{
    Super::DrawHUD();
	DrawSelector();
}

void ABaseHUD::DrawSelector()
{
	if (!bDraging)
	{
		return;
	}

	EndScreenPos = StartScreenPos;
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	const FVector EndPosProjection = Project(HitResult.Location);
	const float Width	= EndPosProjection.X - StartScreenPos.X;
	const float Height	= EndPosProjection.Y - StartScreenPos.Y;
	DrawRect(SelectorColor, StartScreenPos.X, StartScreenPos.Y, Width, Height);
	EndScreenPos = FVector2D(EndPosProjection.X, EndPosProjection.Y);
}

void ABaseHUD::InitDrag(APlayerController* CurrentPC)
{
	CurrentPC->GetMousePosition(StartScreenPos.X, StartScreenPos.Y);
	bDraging = true;
}

void ABaseHUD::StopDrag()
{
	bDraging = false;
}

void ABaseHUD::RestartScreenPositions()
{
	StartScreenPos	= FVector2D::ZeroVector;
	EndScreenPos	= FVector2D::ZeroVector;
}