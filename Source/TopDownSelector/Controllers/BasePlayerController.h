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
	bool RotatingCamera;
	void LeftClickPressed();
	void LeftClickReleased();
	void RightClickReleased();
	/*void RightClick();
	void RotationControlOpen();//Camera rotation
	void RotationControlClose();//Camera rotation*/

	//Camera Inputs
	void CameraMoveForward(float AxisValue);
	void CameraMoveRight(float AxisValue);
	void CameraYawCamera(float AxisValue);

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
	/*TArray<ABaseAIController*> SelectedControllers;
	FVector StartHitPos = FVector(0.0f, 0.0f, 0.0f);

	void UnSelect();
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
	bool Is2DVectorInRectangle(FVector2D Point, FVector2D RectangleStart, FVector2D RectangleEnd, float radiusCapsule);
	double PointDistance(float X1, float V1, float X2, float V2);
	ABaseAIController* SimpleSelection();*/
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	/*UPROPERTY()
	TArray<ABaseAIController*> UnitsSelected;*/
};
