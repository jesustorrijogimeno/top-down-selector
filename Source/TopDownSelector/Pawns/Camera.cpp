// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent	= CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraSpringArm->bEnableCameraLag	= true;
	CameraSpringArm->bDoCollisionTest	= false;
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, InitHeight), FRotator(-InitAngle, 0.0f, 0.0f));
	CameraSpringArm->TargetArmLength = InitDistance;
	CameraSpringArm->CameraLagSpeed = CameraLag;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->AttachToComponent(CameraSpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, InitHeight), FRotator(-InitAngle, 0.0f, 0.0f));
	CameraSpringArm->TargetArmLength	= InitDistance;
	CameraSpringArm->CameraLagSpeed		= CameraLag;
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}