// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "TopDownSelector/Controllers/BasePlayerController.h"

#include "Camera.generated.h"

UCLASS()
class TOPDOWNSELECTOR_API ACamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACamera();
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float InitAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float CameraLag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float InitHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float InitDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float ZoomOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float ZoomSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float MaxZoomOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float MaxZoomIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraProperties")
	float ScreenPercent;
	UPROPERTY()
	USpringArmComponent* CameraSpringArm;
	UPROPERTY()
	UCameraComponent* Camera;
	bool bRotationGateOpened;

	void StartRotation();
	void StopRotation();
	void ZoomIn();
	void ZoomOut();
	void YawCamera(float AxisValue);
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY()
	ABasePlayerController* CurrentPC;
	UPROPERTY()
	ABaseHUD* CurrentHUD;
	FVector2D MousePosition;
	FVector2D MovementInput;
	float RotationInput;
	float CurrentZoom;

	void ManageRotation(float DeltaTime);
	void ManageZoom(float DeltaTime);
	void ManageMovement(float DeltaTime);
	void MovementWithKeyboard(float DeltaTime);
	void MovementWithMouse(float DeltaTime);
};
