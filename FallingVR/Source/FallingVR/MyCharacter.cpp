// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "MotionControllerComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	MyCamera->AttachTo(RootComponent);

	ControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	ControllerRight->AttachTo(MyCamera);
	ControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	ControllerLeft->AttachTo(MyCamera);

	RightControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightControllerMesh"));
	RightControllerMesh->AttachTo(ControllerRight);
	RightControllerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftControllerMesh"));
	LeftControllerMesh->AttachTo(ControllerLeft);
	LeftControllerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TriggerLeft", IE_Pressed, this, &AMyCharacter::TriggerLeftPressed);
	PlayerInputComponent->BindAction("TriggerLeft", IE_Released, this, &AMyCharacter::TriggerLeftReleased);
	PlayerInputComponent->BindAction("TriggerRight", IE_Pressed, this, &AMyCharacter::TriggerRightPressed);
	PlayerInputComponent->BindAction("TriggerRight", IE_Released, this, &AMyCharacter::TriggerRightReleased);

	PlayerInputComponent->BindAction("FaceButton", IE_Pressed, this, &AMyCharacter::FaceButtonPressed);
	PlayerInputComponent->BindAction("FaceButton", IE_Released, this, &AMyCharacter::FaceButtonReleased);
}

void AMyCharacter::TriggerLeftPressed()
{

}
void AMyCharacter::TriggerLeftReleased()
{

}

void AMyCharacter::TriggerRightPressed()
{

}
void AMyCharacter::TriggerRightReleased()
{

}

void AMyCharacter::FaceButtonPressed()
{

}

void AMyCharacter::FaceButtonReleased()
{

}