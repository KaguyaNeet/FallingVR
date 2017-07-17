// Fill out your copyright notice in the Description page of Project Settings.

#include "MyItemBase.h"
#include "MyCharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyItemBase::AMyItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AMyItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyItemBase::InitItem(AMyCharacterBase* Owner_)
{
	SetOwner(Owner_);
}

void AMyItemBase::DestroyItem()
{

}

AMyCharacterBase* AMyItemBase::GetOwner()
{
	return Owner;
}

bool AMyItemBase::SetOwner(AMyCharacterBase* Owner_)
{
	if (Owner_)
	{
		Owner = Owner_;
		return true;
	}
	else
	{
		return false;
	}
}

void AMyItemBase::UseItem()
{
	if (AllowUse)
	{
		if (!isCD)
		{
			isCD = true;
			GetWorldTimerManager().SetTimer(UseRateTimerHandle, this, &AMyItemBase::InitCD, CDtime);
			ItemFunction();
		}
	}
}

void AMyItemBase::InitCD()
{
	isCD = false;
}

void AMyItemBase::SetAllowUse(bool Choose)
{
	AllowUse = Choose;
}

void AMyItemBase::ItemFunction()
{

}
