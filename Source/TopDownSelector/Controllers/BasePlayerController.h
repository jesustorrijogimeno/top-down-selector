// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../HUD/BaseHUD.h"

#include "AIControllers/BaseAIController.h"
#include "GameFramework/PlayerController.h"
#include "TopDownSelector/GameModes/GameModeMain.h"

#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSELECTOR_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	ABaseHUD* BaseHUD;
	UPROPERTY()
	AGameModeMain* CurrentGM;

	bool bRotatingCamera;
	void LeftClickPressed();
	void LeftClickReleased();
	void RightClickReleased();

	//Camera Inputs
	void CameraMoveForward(float AxisValue);
	void CameraMoveRight(float AxisValue);
	void CameraYawCamera(float AxisValue);

	void RotationControlOpen();
	void RotationControlClose();

	void CameraZoomIn();
	void CameraZoomOut();
	void MultipleSelection() const;
	void SingleSelection() const;

	ABaseAIController* GetAIControllerSelected() const;
	static double PointDistance(float X1, float X2, float Y1, float Y2);
	static bool Is2DVectorInRectangle(FVector2D Point, FVector2D RectangleStart, FVector2D RectangleEnd, float RadiusCapsule);
	static bool IsOutSideRight(FVector2D Point, FVector2D VectorToCompare);
	static bool IsOutSideLeft(FVector2D Point, FVector2D VectorToCompare);
	static bool IsOutSideTop(FVector2D Point, FVector2D VectorToCompare);
	static bool IsOutSideBottom(FVector2D Point, FVector2D VectorToCompare);
public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	void SetMouseActive(bool bActivated);
};