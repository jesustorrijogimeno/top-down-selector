// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"

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

void ABasePlayerController::MultipleSelection()
{
	CurrentGM->UnitsSelected = BaseHUD->GetActorsInDrag();
}

void ABasePlayerController::SingleSelection() const
{
	if (!(BaseHUD->StartScreenPos - BaseHUD->EndScreenPos).IsNearlyZero()
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

ABaseAIController* ABasePlayerController::GetAIControllerSelected() const
{
	FHitResult Result(ForceInit);
	GetHitResultUnderCursor(ECC_WorldDynamic, false, Result);
	if (Result.GetActor() == nullptr || Result.GetActor()->GetInstigatorController() == nullptr) {
		return nullptr;
	}

	ABaseAIController* CurrentController = Cast<ABaseAIController>(Result.GetActor()->GetInstigatorController());
	if (CurrentController == nullptr) {
		return nullptr;
	}
	return CurrentController;
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