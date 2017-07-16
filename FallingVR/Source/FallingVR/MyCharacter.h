// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGVR_API AMyCharacter : public AMyCharacterBase
{
	GENERATED_BODY()
	
public:

	AMyCharacter();

	void TriggerRightPressed();
	void TriggerRightReleased();
	void TriggerLeftPressed();
	void TriggerLeftReleased();

	void FaceButtonPressed();
	void FaceButtonReleased();
	
public:
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* MyCamera;
	UPROPERTY(VisibleAnywhere)
		class UMotionControllerComponent* ControllerRight;
	UPROPERTY(VisibleAnywhere)
		class UMotionControllerComponent* ControllerLeft;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RightControllerMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftControllerMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
