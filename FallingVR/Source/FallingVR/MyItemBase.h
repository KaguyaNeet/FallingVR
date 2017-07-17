// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyItemBase.generated.h"

UCLASS()
class FALLINGVR_API AMyItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItemBase();

	virtual void DestroyItem();
	virtual void InitItem(class AMyCharacterBase* Owner_);

	void UseItem();
	virtual void ItemFunction();

	void SetAllowUse(bool Choose);

	class AMyCharacterBase* GetOwner();
	bool SetOwner(class AMyCharacterBase* Owner_);

public:

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		float CDtime = 1.f;

private:

	class AMyCharacterBase* Owner = NULL;
	FTimerHandle UseRateTimerHandle;
	bool isCD = false;
	bool AllowUse = true;

private:

	void InitCD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
