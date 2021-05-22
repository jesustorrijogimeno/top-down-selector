// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownSelector/Controllers/AIControllers/BaseAIController.h"

#include "GameModeMain.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSELECTOR_API AGameModeMain : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Units Selected")
	TArray<ABaseAIController*> UnitsSelected;
};
