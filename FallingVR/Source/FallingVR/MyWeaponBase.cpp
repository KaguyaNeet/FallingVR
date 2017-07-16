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
	//���ж��Ƿ��������
	if (AllowFire)
	{
		//�ж��Ƿ������
		if (CanFire)
		{
			//�������Ƶ��
			CanFire = false;
			GetWorldTimerManager().SetTimer(FireRateTimeHandle, this, &AMyWeaponBase::InitCanFire, FireRate);
			//����������Ч��������ײ�Ĳ���
			FHitResult TraceHit;
			FVector LineStart = MuzzleArrow->GetComponentLocation();
			FVector LineEnd = LineStart + MuzzleArrow->GetForwardVector() * FireDistance;
			FTransform MuzzleTransform = MuzzleArrow->GetComponentTransform();
			UWorld* MyWorld = GetWorld();
			//����ǹ��������Ч
			if (MuzzleParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(MyWorld, MuzzleParticle, MuzzleTransform, true);
			}
			//����������ײ�ж�
			if (MyWorld->LineTraceSingleByChannel(TraceHit, LineStart, LineEnd, ECollisionChannel::ECC_WorldDynamic))
			{
				AddDamage(BaseDamageValue, TraceHit, Owner);
			}
		}
	}
}
//ʩ���˺�
void AMyWeaponBase::AddDamage(float DamageValue, FHitResult& HitResult, AMyCharacterBase* Causer)
{
	if (AMyCharacterBase* HitCharacter = Cast<AMyCharacterBase>(HitResult.GetActor()))
	{
		HitCharacter->ApplyDamage(DamageValue, Causer, HitResult);
	}
}