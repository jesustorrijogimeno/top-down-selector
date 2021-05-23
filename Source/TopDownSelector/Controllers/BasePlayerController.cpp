// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"

#include <iterator>

#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "TopDownSelector/Pawns/AICharacter.h"
#include "TopDownSelector/Pawns/Camera.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetMouseActive(true);
	bRotatingCamera	= false;

	BaseHUD		= CastChecked<ABaseHUD>(GetHUD());
	CurrentGM	= CastChecked<AGameModeMain>(GetWorld()->GetAuthGameMode());
}


void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//Hook up events for "Clicks"
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ABasePlayerController::LeftClickPressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &ABasePlayerController::LeftClickReleased);
	InputComponent->BindAction("RightClick", IE_Released, this, &ABasePlayerController::RightClickReleased);

	//Camera Inputs
	//-Hook Rotation camera
	InputComponent->BindAction("CameraRotationControl", IE_Pressed, this, &ABasePlayerController::RotationControlOpen);

	//-Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABasePlayerController::CameraZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ABasePlayerController::CameraZoomOut);

	//-Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::CameraMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::CameraMoveRight);
	InputComponent->BindAxis("CameraYaw", this, &ABasePlayerController::CameraYawCamera);
}

void ABasePlayerController::LeftClickPressed()
{
	this->BaseHUD->InitDrag(this);
	this->CurrentGM->UnSelectAllUnits();
}

void ABasePlayerController::LeftClickReleased()
{
	this->BaseHUD->StopDrag();
	MultipleSelection();
	SingleSelection();
	this->BaseHUD->RestartScreenPositions();
}

void ABasePlayerController::RightClickReleased()
{
	if (this->bRotatingCamera || this->CurrentGM->UnitsSelected.Num() == 0) 
	{
		RotationControlClose();
		return;
	}
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit) 
	{
		// We hit something, move there
		for (int32 i = 0; i < this->CurrentGM->UnitsSelected.Num(); i++)
		{
			this->CurrentGM->UnitsSelected[i]->MoveCharacterTo(Hit.ImpactPoint);
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
		if (Is2DVectorInRectangle(ActorLocation2D, this->BaseHUD->StartScreenPos, this->BaseHUD->EndScreenPos, Distance))
		{
			CastChecked<AAICharacter>(AIController->GetPawn())->OnSelected();
			this->CurrentGM->UnitsSelected.Add(AIController);
		}
	}
}

void ABasePlayerController::SingleSelection() const
{
	if (this->CurrentGM->UnitsSelected.Num() != 0)
	{
		return;
	}

	ABaseAIController* CurrentController = GetAIControllerSelected();
	if (CurrentController == nullptr) 
	{
		return;
	}
	CastChecked<AAICharacter>(CurrentController->GetPawn())->OnSelected();
	this->CurrentGM->UnitsSelected.Add(CurrentController);
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

double ABasePlayerController::PointDistance(
	const float X1, 
	const float X2, 
	const float Y1, 
	const float Y2)
{
	return sqrt(pow((X2 - X1), 2) + pow((Y2 - Y1), 2));
}

bool ABasePlayerController::Is2DVectorInRectangle(
	const FVector2D Point, 
	const FVector2D RectangleStart, 
	const FVector2D RectangleEnd, 
	const float RadiusCapsule)
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

bool ABasePlayerController::IsOutSideLeft(
	const FVector2D Point, 
	const FVector2D VectorToCompare)
{
	return Point.X < VectorToCompare.X;
}

bool ABasePlayerController::IsOutSideRight(
	const FVector2D Point, 
	const FVector2D VectorToCompare)
{
	return Point.X > VectorToCompare.X;
}

bool ABasePlayerController::IsOutSideTop(
	const FVector2D Point, 
	const FVector2D VectorToCompare)
{
	return Point.Y > VectorToCompare.Y;
}

bool ABasePlayerController::IsOutSideBottom(
	const FVector2D Point, 
	const FVector2D VectorToCompare)
{
	return Point.Y < VectorToCompare.Y;
}

void ABasePlayerController::SetMouseActive(const bool bActivated)
{
	bShowMouseCursor		= bActivated;
	bEnableClickEvents		= bActivated;
	bEnableMouseOverEvents	= bActivated;
}

//Camera Inputs
void ABasePlayerController::CameraMoveForward(const float AxisValue)
{
	CastChecked<ACamera>(GetPawn())->MoveForward(AxisValue);
}

void ABasePlayerController::CameraMoveRight(const float AxisValue)
{
	CastChecked<ACamera>(GetPawn())->MoveRight(AxisValue);
}

void ABasePlayerController::CameraYawCamera(const float AxisValue)
{
	CastChecked<ACamera>(GetPawn())->YawCamera(AxisValue);
}

void ABasePlayerController::CameraZoomIn()
{
	CastChecked<ACamera>(GetPawn())->ZoomIn();
}

void ABasePlayerController::CameraZoomOut()
{
	CastChecked<ACamera>(GetPawn())->ZoomOut();
}

void ABasePlayerController::RotationControlClose()
{
	this->bRotatingCamera = false;
	CastChecked<ACamera>(GetPawn())->StopRotation();
}

void ABasePlayerController::RotationControlOpen()
{
	this->bRotatingCamera = true;
	CastChecked<ACamera>(GetPawn())->StartRotation();
}