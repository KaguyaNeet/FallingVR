// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWeapon.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacterBase.h"
#include "MyBulletBase.h"
#include "MyCharacter.h"

AMyWeapon::AMyWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));
	MuzzleArrow->AttachTo(WeaponMesh);

	HandArrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow1"));
	HandArrow1->AttachTo(WeaponMesh);

	HandArrow2 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow2"));
	HandArrow2->AttachTo(WeaponMesh);

}

void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GetOwner()) && Cast<AMyCharacter>(GetOwner())->GetRunningFireWeaponOn() && RunningFire)
	{
		UseItem();
	}
}

void AMyWeapon::InitItem(class AMyCharacterBase* Owner_, EMyWeapon::Type NewType)
{
	Super::InitItem(Owner_);
}

void AMyWeapon::ItemFunction()
{
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
	//������������ִ����Ӧ���ӵ����ɷ�ʽ
	switch (MyWeaponType)
	{
	case EMyWeapon::Pistol:BulletIsProjectle(); break;
	case EMyWeapon::MiniGun:BulletIsSingleTrace(); break;
	case EMyWeapon::Rifle:BulletIsProjectle(); break;
	case EMyWeapon::Sniper:BulletIsProjectle(); break;
	case EMyWeapon::Shotgun:BulletIsMultiTrace(); break;
	}
}

void AMyWeapon::AddDamage(float DamageValue, const FHitResult& HitResult, AMyCharacterBase* Causer)
{
	if (AMyCharacterBase* HitCharacter = Cast<AMyCharacterBase>(HitResult.GetActor()))
	{
		HitCharacter->ApplyDamage(DamageValue, Causer, HitResult);
	}
}

void AMyWeapon::BulletIsProjectle()
{
	UWorld* MyWorld = GetWorld();
	AMyBulletBase* TempBullet = MyWorld->SpawnActor<AMyBulletBase>(MuzzleArrow->GetComponentLocation(), MuzzleArrow->GetComponentRotation());
	TempBullet->BeginInit(GetOwner(), MyWeaponType, GetOwner()->MyElementType);
}
void AMyWeapon::BulletIsSingleTrace()
{
	FHitResult TraceHit;
	FVector LineStart = MuzzleArrow->GetComponentLocation();
	FVector LineEnd = LineStart + MuzzleArrow->GetForwardVector() * FireDistance;
	FTransform BulletTransform = MuzzleArrow->GetComponentTransform();
	UWorld* MyWorld = GetWorld();

	UGameplayStatics::SpawnEmitterAtLocation(MyWorld, LineTraceBulletParticle, BulletTransform);
	if (MyWorld->LineTraceSingleByChannel(TraceHit, LineStart, LineEnd, ECollisionChannel::ECC_WorldDynamic))
	{
		AddDamage(BaseDamageValue, TraceHit, GetOwner());
	}
}
void AMyWeapon::BulletIsMultiTrace()
{
	FHitResult TraceHit;
	FVector LineStart = MuzzleArrow->GetComponentLocation();
	UWorld* MyWorld = GetWorld();

	for (int i = 0; i < ShotGunBulletNum; i++)
	{
		MuzzleArrow->SetRelativeRotation(FRotator(0.f, FMath::RandRange(-10.f, 10.f), FMath::RandRange(-10.f, 10.f)));
		FTransform BulletTransform = MuzzleArrow->GetComponentTransform();
		FVector LineEnd = LineStart + MuzzleArrow->GetForwardVector() * FireDistance;
		UGameplayStatics::SpawnEmitterAtLocation(MyWorld, LineTraceBulletParticle, BulletTransform);
		if (MyWorld->LineTraceSingleByChannel(TraceHit, LineStart, LineEnd, ECollisionChannel::ECC_WorldDynamic))
		{
			AddDamage(BaseDamageValue, TraceHit, GetOwner());
		}
	}
	MuzzleArrow->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}


