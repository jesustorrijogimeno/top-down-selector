// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Controllers/AIControllers/BaseAIController.h"

#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSELECTOR_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	bool bDraging = false;

	UPROPERTY(EditAnywhere, Category="Selector Config")
	FLinearColor SelectorColor;
	UPROPERTY(EditAnywhere, Category = "Selector Config")
	TArray<ABaseAIController*> UnitsSelected;
	virtual void DrawHUD() override;
	void InitDrag(APlayerController* CurrentPC);
	void StopDrag();
	FVector2D StartScreenPos;
	FVector2D EndScreenPos;
	void RestartScreenPositions();
private:
	void DrawSelector();
};
