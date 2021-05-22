// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"

#include <iterator>

#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor		= true;
	bEnableClickEvents		= true;
	bEnableMouseOverEvents	= true;
	RotatingCamera			= false;

	BaseHUD			= CastChecked<ABaseHUD>(GetHUD());
	CurrentGM		= CastChecked<AGameModeMain>(GetWorld()->GetAuthGameMode());
}


void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//Hook up events for "Clicks"
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ABasePlayerController::LeftClickPressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &ABasePlayerController::LeftClickReleased);
	InputComponent->BindAction("RightClick", IE_Released, this, &ABasePlayerController::RightClickReleased);
	/*InputComponent->BindAction("RightClick", IE_Released, this, &ABasePlayerController::RightClick);

	//Camera Inputs
	//-Hook Rotation camera
	InputComponent->BindAction("CameraRotationControl", IE_Pressed, this, &ABasePlayerController::RotationControlOpen);
	InputComponent->BindAction("CameraRotationControl", IE_Released, this, &ABasePlayerController::RotationControlClose);

	//-Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABasePlayerController::CameraZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ABasePlayerController::CameraZoomOut);

	//-Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::CameraMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::CameraMoveRight);
	InputComponent->BindAxis("CameraYaw", this, &ABasePlayerController::CameraYawCamera);*/

}

void ABasePlayerController::LeftClickPressed()
{
	BaseHUD->InitDrag(this);
	CurrentGM->UnitsSelected.Empty();
}

void ABasePlayerController::LeftClickReleased()
{
	BaseHUD->StopDrag();
	MultipleSelection();
	SingleSelection();
	BaseHUD->RestartScreenPositions();
}

void ABasePlayerController::RightClickReleased()
{
	if (!RotatingCamera && CurrentGM->UnitsSelected.Num() != 0) {
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit) {
			// We hit something, move there
			for (int32 i = 0; i < CurrentGM->UnitsSelected.Num(); i++) {
				CurrentGM->UnitsSelected[i]->MoveCharacterTo(Hit.ImpactPoint);
			}
		}
	}
}

void ABasePlayerController::MultipleSelection() const
{
	for (ABaseAIController* AIController : TActorRange<ABaseAIController> (GetWorld()))
	{
		FVector2D ActorLocation2D;
		FVector ActorLocation = AIController->GetPawn()->GetActorLocation();
		const float RadiusCapsule = (AIController->GetCharacter())->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
		ProjectWorldLocationToScreen(ActorLocation, ActorLocation2D);

		FVector2D RadiusPoint;
		ProjectWorldLocationToScreen(ActorLocation + FVector(RadiusCapsule, 0, 0), RadiusPoint);
		const float Distance = PointDistance(ActorLocation2D.X, RadiusPoint.X, ActorLocation2D.Y, RadiusPoint.Y);
		if (Is2DVectorInRectangle(ActorLocation2D, BaseHUD->StartScreenPos, BaseHUD->EndScreenPos, Distance)) 
		{
			AIController->bSelected = true;
			CurrentGM->UnitsSelected.Add(AIController);
		}
	}
}

void ABasePlayerController::SingleSelection() const
{
	if (CurrentGM->UnitsSelected.Num() != 0) 
	{
		return;
	}

	ABaseAIController* CurrentController = GetAIControllerSelected();
	if (CurrentController == nullptr) 
	{
		return;
	}

	CurrentController->bSelected = true;
	CurrentGM->UnitsSelected.Add(CurrentController);
}

ABaseAIController* ABasePlayerController::GetAIControllerSelected() const
{
	FHitResult Result(ForceInit);
	GetHitResultUnderCursor(ECC_WorldDynamic, false, Result);
	if (Result.GetActor() == nullptr || Result.GetActor()->GetInstigatorController() == nullptr) 
	{
		return nullptr;
	}

	ABaseAIController* CurrentController = Cast<ABaseAIController>(Result.GetActor()->GetInstigatorController());
	if (CurrentController == nullptr) 
	{
		return nullptr;
	}
	return CurrentController;
}

double ABasePlayerController::PointDistance(float X1, float X2, float Y1, float Y2)
{
	return sqrt(pow((X2 - X1), 2) + pow((Y2 - Y1), 2));
}

bool ABasePlayerController::Is2DVectorInRectangle(FVector2D Point, FVector2D RectangleStart, FVector2D RectangleEnd, float RadiusCapsule)
{
	FVector2D Min;
	FVector2D Max;

	Min.X = fminf(RectangleStart.X, RectangleEnd.X);
	Min.Y = fminf(RectangleStart.Y, RectangleEnd.Y);

	Max.X = fmaxf(RectangleStart.X, RectangleEnd.X);
	Max.Y = fmaxf(RectangleStart.Y, RectangleEnd.Y);

	if (!IsOutSideRight(Point, Max) 
		&& !IsOutSideLeft(Point, Min) 
		&& !IsOutSideTop(Point, Max)
		&& !IsOutSideBottom(Point, Min))
	{
		//Totally inside
		return true;
	}

	if (IsOutSideTop(Point, Max) && IsOutSideLeft(Point, Min))
	{
        return RadiusCapsule > PointDistance(Point.X, Point.Y, Min.X, Max.Y);
	}

    if (IsOutSideTop(Point, Max) && IsOutSideRight(Point, Max))
	{
		return RadiusCapsule > PointDistance(Point.X, Point.Y, Max.X, Max.Y);
	}

    if (IsOutSideBottom(Point, Min) && IsOutSideLeft(Point, Min))
	{
		return RadiusCapsule > PointDistance(Point.X, Point.Y, Min.X, Min.Y);
	}

    if (IsOutSideBottom(Point, Min) && IsOutSideRight(Point, Max))
	{
		return RadiusCapsule > PointDistance(Point.X, Point.Y, Max.Y, Min.Y);
	}

    if (IsOutSideTop(Point, Max) && !IsOutSideRight(Point, Max) && !IsOutSideLeft(Point, Min))
	{
		return RadiusCapsule > (Point.Y - Max.Y);
	}
	
	if (IsOutSideBottom(Point, Min) && !IsOutSideRight(Point, Max) && !IsOutSideLeft(Point, Min))
	{
		return RadiusCapsule > (Min.Y - Point.Y);
	}

	if (!IsOutSideTop(Point, Max) && !IsOutSideBottom(Point, Min) && IsOutSideLeft(Point, Min))
	{
		return RadiusCapsule > (Min.X - Point.X);
	}

	if (!IsOutSideTop(Point, Max) && !IsOutSideBottom(Point, Min) && IsOutSideRight(Point, Max))
	{
		return RadiusCapsule > (Point.X - Max.X);
	}
    return false;
}

bool ABasePlayerController::IsOutSideLeft(FVector2D Point, FVector2D VectorToCompare)
{
	return Point.X < VectorToCompare.X;
}

bool ABasePlayerController::IsOutSideRight(FVector2D Point, FVector2D VectorToCompare)
{
	return Point.X > VectorToCompare.X;
}

bool ABasePlayerController::IsOutSideTop(FVector2D Point, FVector2D VectorToCompare)
{
	return Point.Y > VectorToCompare.Y;
}

bool ABasePlayerController::IsOutSideBottom(FVector2D Point, FVector2D VectorToCompare)
{
	return Point.Y < VectorToCompare.Y;
}

//Camera Inputs
void ABasePlayerController::CameraMoveForward(float AxisValue) {
	//CastChecked<ACameraC>(GetPawn())->MoveForward(AxisValue);
}

void ABasePlayerController::CameraMoveRight(float AxisValue) {
	//CastChecked<ACameraC>(GetPawn())->MoveRight(AxisValue);
}

void ABasePlayerController::CameraYawCamera(float AxisValue) {
	//CastChecked<ACameraC>(GetPawn())->YawCamera(AxisValue);
}

void ABasePlayerController::CameraZoomIn() {
	//CastChecked<ACameraC>(GetPawn())->ZoomIn();
}

void ABasePlayerController::CameraZoomOut() {
	//CastChecked<ACameraC>(GetPawn())->ZoomOut();
}