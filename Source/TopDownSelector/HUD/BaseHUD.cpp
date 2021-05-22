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
	//GetActorsInDrag();
	//ManageSelection();
}

/*void ABaseHUD::ManageSelection()
{
	AGameModeMain* CurrentGM = CastChecked<AGameModeMain>(GetWorld()->GetAuthGameMode());
	CurrentGM->UnitsSelected.Empty();
	MultipleSelection(CurrentGM);
	SingleSelection(CurrentGM);
	RestartScreenPositions();
}*/

/*void ABaseHUD::GetActorsInDrag()
{
	UnitsSelected.Empty();
	TArray<ABaseAIController*> UnitSelected;
	TArray<AActor*> Actors;
	GetActorsInSelectionRectangle(StartScreenPos, EndScreenPos, Actors);
    for (auto Actor : Actors)
    {
		AController* Current = Actor->GetInstigatorController();
		ABaseAIController* AIController = Cast<ABaseAIController>(Current);
		if (AIController != nullptr)
		{
			UnitsSelected.Add(AIController);
		}
    }
}*/

/*void ABaseHUD::SingleSelection(AGameModeMain* CurrentGM) const
{
	if (!(StartScreenPos - EndScreenPos).IsNearlyZero()
		|| CurrentGM->UnitsSelected.Num() != 0) {
		return;
	}

	ABaseAIController* CurrentController = GetAIControllerSelected();
	if (CurrentController == nullptr) {
		return;
	}
	CurrentController->bSelected = true;
	CurrentGM->UnitsSelected.Add(CurrentController);
}

ABaseAIController* ABaseHUD::GetAIControllerSelected() const
{
	FHitResult Result(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_WorldDynamic, false, Result);
	if (Result.GetActor() == nullptr || Result.GetActor()->GetInstigatorController() == nullptr) {
		return nullptr;
	}

	ABaseAIController* CurrentController = Cast<ABaseAIController>(Result.GetActor()->GetInstigatorController());
	if (CurrentController == nullptr) {
		return nullptr;
	}
	return CurrentController;
}

void ABaseHUD::InitDrag()
{
	bLeftClicked = true;
	GetOwningPlayerController()->GetMousePosition(StartScreenPos.X, StartScreenPos.Y);
}

void ABaseHUD::StopDrag()
{
	bLeftClicked = false;
}*/

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