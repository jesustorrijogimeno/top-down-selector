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
	if (!this->bDraging)
	{
		return;
	}

	this->EndScreenPos = this->StartScreenPos;
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	const FVector EndPosProjection = Project(HitResult.Location);
	const float Width	= EndPosProjection.X - this->StartScreenPos.X;
	const float Height	= EndPosProjection.Y - this->StartScreenPos.Y;
	DrawRect(this->SelectorColor, this->StartScreenPos.X, this->StartScreenPos.Y, Width, Height);
	this->EndScreenPos = FVector2D(EndPosProjection.X, EndPosProjection.Y);
}

void ABaseHUD::InitDrag(APlayerController* CurrentPC)
{
	CurrentPC->GetMousePosition(this->StartScreenPos.X, this->StartScreenPos.Y);
	this->bDraging = true;
}

void ABaseHUD::StopDrag()
{
	this->bDraging = false;
}

void ABaseHUD::RestartScreenPositions()
{
	this->StartScreenPos	= FVector2D::ZeroVector;
	this->EndScreenPos		= FVector2D::ZeroVector;
}