// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	this->CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	this->CameraSpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	this->CameraSpringArm->bEnableCameraLag	= true;
	this->CameraSpringArm->bDoCollisionTest	= false;
	this->CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, this->InitHeight), FRotator(-this->InitAngle, 0.0f, 0.0f));
	this->CameraSpringArm->TargetArmLength	= this->InitDistance;
	this->CameraSpringArm->CameraLagSpeed	= this->CameraLag;
	this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	this->Camera->AttachToComponent(this->CameraSpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();
	this->CurrentPC		= CastChecked<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	this->CurrentHUD	= CastChecked<ABaseHUD>(this->CurrentPC->GetHUD());
	this->CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, this->InitHeight), FRotator(-this->InitAngle, 0.0f, 0.0f));
	this->CameraSpringArm->TargetArmLength	= this->InitDistance;
	this->CameraSpringArm->CameraLagSpeed	= this->CameraLag;
	this->CurrentZoom						= this->InitDistance;
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageRotation(DeltaTime);
	ManageMovement(DeltaTime);
	ManageZoom(DeltaTime);
}

// Called to bind functionality to input
void ACamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ACamera::ManageRotation(const float DeltaTime)
{
	if (!this->bRotationGateOpened)
	{
		return;
	}

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += this->RotationInput * DeltaTime;
	SetActorRotation(NewRotation);
	
	FViewport* FViewportObject = CastChecked<ULocalPlayer>(this->CurrentPC->Player)->ViewportClient->Viewport;
	FViewportObject->SetMouse(this->MousePosition.X, this->MousePosition.Y);
}

void ACamera::ManageMovement(const float DeltaTime)
{
	if (this->CurrentHUD->bDraging) {
		return;
	}
	MovementWithKeyboard(DeltaTime);
	MovementWithMouse(DeltaTime);
}

void ACamera::MovementWithKeyboard(const float DeltaTime)
{
	if (this->MovementInput.IsZero()) 
	{
		return;
	}

	this->MovementInput = this->MovementInput.GetSafeNormal() * this->MovementSpeed;
	FVector NewLocation = GetActorLocation();
	NewLocation += GetActorForwardVector() * this->MovementInput.X * DeltaTime;
	NewLocation += GetActorRightVector() * this->MovementInput.Y * DeltaTime;
	SetActorLocation(NewLocation);
}

void ACamera::MovementWithMouse(const float DeltaTime)
{
	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

	CurrentPC->GetMousePosition(this->MousePosition.X, this->MousePosition.Y);

	FVector NewLocation = GetActorLocation();
	// Horizontal movement
	if (this->MousePosition.X > (ViewportSize.X * (1 - this->ScreenPercent / 100.0f)))
	{
		NewLocation += GetActorRightVector() * this->MovementSpeed * DeltaTime;
	}
	else if (this->MousePosition.X < (ViewportSize.X * this->ScreenPercent / 100.0f))
	{
		NewLocation -= GetActorRightVector() * this->MovementSpeed * DeltaTime;
	}
	// Vertical movement
	if (this->MousePosition.Y > (ViewportSize.Y * (1 - this->ScreenPercent / 100.0f)))
	{
		NewLocation -= GetActorForwardVector() * this->MovementSpeed * DeltaTime;
	}
	else if (this->MousePosition.Y < (ViewportSize.Y * this->ScreenPercent / 100.0f))
	{
		NewLocation += GetActorForwardVector() * this->MovementSpeed * DeltaTime;
	}
	SetActorLocation(NewLocation);
}

void ACamera::ManageZoom(const float DeltaTime)
{
	if (this->CameraSpringArm->TargetArmLength > this->CurrentZoom) {
		this->CameraSpringArm->TargetArmLength =
			FMath::Clamp<float>(this->CameraSpringArm->TargetArmLength - this->ZoomSpeed * DeltaTime,
				this->CurrentZoom, this->CameraSpringArm->TargetArmLength);
		return;
	}

	if (this->CameraSpringArm->TargetArmLength < this->CurrentZoom) {
		this->CameraSpringArm->TargetArmLength =
			FMath::Clamp<float>(this->CameraSpringArm->TargetArmLength + this->ZoomSpeed * DeltaTime,
				this->CameraSpringArm->TargetArmLength, this->CurrentZoom);
	}
}

void ACamera::StartRotation()
{
	this->CurrentPC->SetMouseActive(false);
	this->bRotationGateOpened = true;
	this->CurrentPC->GetMousePosition(this->MousePosition.X, this->MousePosition.Y);
}

void ACamera::StopRotation()
{
	this->CurrentPC->SetMouseActive(true);
	this->bRotationGateOpened = false;
}

void ACamera::YawCamera(const float AxisValue)
{
	if (!this->bRotationGateOpened)
	{
		return;
	}
	this->RotationInput = AxisValue * this->RotationSpeed;
}

void ACamera::MoveForward(const float AxisValue)
{
	this->MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ACamera::MoveRight(const float AxisValue)
{
	this->MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ACamera::ZoomIn()
{
	if ((this->CurrentZoom - this->ZoomOffset) > this->MaxZoomIn)
	{
		this->CurrentZoom -= this->ZoomOffset;
	}
}

void ACamera::ZoomOut()
{
	if ((this->CurrentZoom + this->ZoomOffset) < this->MaxZoomOut)
	{
		this->CurrentZoom += this->ZoomOffset;
	}
}