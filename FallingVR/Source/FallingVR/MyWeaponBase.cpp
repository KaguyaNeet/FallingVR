// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacterBase.h"

// Sets default values
AMyWeaponBase::AMyWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));
	MuzzleArrow->AttachTo(WeaponMesh);

	HandArrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow1"));
	HandArrow1->AttachTo(WeaponMesh);

	HandArrow2 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow2"));
	HandArrow2->AttachTo(WeaponMesh);
}

// Called when the game starts or when spawned
void AMyWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeaponBase::BeginInit()
{

}

void AMyWeaponBase::DestroyWeapon()
{

}

void AMyWeaponBase::InitCanFire()
{
	CanFire = true;
}
void AMyWeaponBase::SetAllowFire(bool Choose)
{
	AllowFire = Choose;
}

void AMyWeaponBase::Fire()
{
	//先判断是否允许射击
	if (AllowFire)
	{
		//判断是否能射击
		if (CanFire)
		{
			//设置射击频率
			CanFire = false;
			GetWorldTimerManager().SetTimer(FireRateTimeHandle, this, &AMyWeaponBase::InitCanFire, FireRate);
			//设置粒子特效和射线碰撞的参数
			FHitResult TraceHit;
			FVector LineStart = MuzzleArrow->GetComponentLocation();
			FVector LineEnd = LineStart + MuzzleArrow->GetForwardVector() * FireDistance;
			FTransform MuzzleTransform = MuzzleArrow->GetComponentTransform();
			UWorld* MyWorld = GetWorld();
			//生成枪口栗子特效
			if (MuzzleParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(MyWorld, MuzzleParticle, MuzzleTransform, true);
			}
			//进行射线碰撞判断
			if (MyWorld->LineTraceSingleByChannel(TraceHit, LineStart, LineEnd, ECollisionChannel::ECC_WorldDynamic))
			{
				AddDamage(BaseDamageValue, TraceHit, Owner);
			}
		}
	}
}
//施加伤害
void AMyWeaponBase::AddDamage(float DamageValue, FHitResult& HitResult, AMyCharacterBase* Causer)
{
	if (AMyCharacterBase* HitCharacter = Cast<AMyCharacterBase>(HitResult.GetActor()))
	{
		HitCharacter->ApplyDamage(DamageValue, Causer, HitResult);
	}
}