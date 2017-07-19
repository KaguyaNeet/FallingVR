// Fill out your copyright notice in the Description page of Project Settings.
#include "MyCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyStructs.h"


// Sets default values
AMyCharacterBase::AMyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//执行伤害
void AMyCharacterBase::ApplyDamage(EMyElement::Type DamageType, float ElementLevel, float DamageValue, AMyCharacterBase* Causer, const FHitResult& HitResult)
{
	//计算伤害
	LifeValue -= DamageValue;

	//计算生成粒子特效的Transform
	UWorld* MyWorld = GetWorld();
	FVector DamageLocation = HitResult.ImpactPoint;
	FVector DamageNormal = HitResult.ImpactNormal;
	FRotator DamageRotation = DamageNormal.Rotation();
	FTransform DamageTransform = FTransform(DamageRotation, DamageLocation, FVector(1.f, 1.f, 1.f));

	//判断是否死亡
	if (LifeValue <= 0)
	{
		ApplyDeath(Causer);
	}
}

//执行死亡（。_。）
void AMyCharacterBase::ApplyDeath(AMyCharacterBase* Causer)
{

}

void AMyCharacterBase::ApplyElementDamage(EMyElement::Type DamageType, float ParamElementLevel)
{
	if (DamageType == EMyElement::Ice)
	{

	}
	if (DamageType == EMyElement::Fire)
	{

	}
	if (DamageType == EMyElement::Poison)
	{

	}
}

float AMyCharacterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0;
}