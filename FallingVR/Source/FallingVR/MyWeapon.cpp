// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWeapon.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacterBase.h"
#include "MyBulletBase.h"
#include "MyCharacter.h"
#include "Engine/DataTable.h"
#include "ConstructorHelpers.h"
#include "MyStructs.h"

AMyWeapon::AMyWeapon()
{
	HandArrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow1"));
	HandArrow1->AttachTo(RootComponent);

	HandArrow2 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow2"));
	HandArrow2->AttachTo(RootComponent);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(HandArrow1);

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));
	MuzzleArrow->AttachTo(WeaponMesh);

	ConstructorHelpers::FObjectFinder<UDataTable> TempData(TEXT("DataTable'/Game/Blueprint/DWeapon.DWeapon'"));
	if (TempData.Succeeded())
	{
		WeaponData = TempData.Object;
	}
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
	if (IsValid(WeaponData))
	{
		MyWeaponType = NewType;
		TArray<FName> WeaponDataName = WeaponData->GetRowNames();
		FWeaponType* MyType = WeaponData->FindRow<FWeaponType>(WeaponDataName[NewType], TEXT(""));
		WeaponMesh->SetSkeletalMesh(MyType->WeaponMesh);
		One_handed = MyType->One_Handed;
		CDtime = MyType->FireCD;
		MuzzleArrow->SetRelativeLocation(MyType->MuzzleOffset);
		WeaponMesh->SetRelativeLocation(MyType->MeshOffset);
		BaseDamageValue = MyType->BaseDamageValue;
		FireDistance = MyType->FireRange;
		MuzzleParticle = MyType->MuzzleParticle;
		LineTraceBulletParticle = MyType->BulletParticle;
		RunningFire = MyType->RunningFire;
		TwoHandMaxDistance = MyType->TwoHandMaxDistance;
		TwoHandMinDistance = MyType->TwoHandMinDistance;
	}

}

void AMyWeapon::ItemFunction()
{
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
	//根据武器种类执行相应的子弹生成方式
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
		MuzzleArrow->SetRelativeRotation(FRotator(FMath::RandRange(-10.f, 10.f), FMath::RandRange(-10.f, 10.f), FMath::RandRange(-10.f, 10.f)));
		FTransform BulletTransform = MuzzleArrow->GetComponentTransform();
		FVector LineEnd = LineStart + MuzzleArrow->GetForwardVector() * FireDistance;
		AMyBulletBase* TempBullet = MyWorld->SpawnActor<AMyBulletBase>(MuzzleArrow->GetComponentLocation(), MuzzleArrow->GetComponentRotation());
		TempBullet->BeginInit(GetOwner(), MyWeaponType, GetOwner()->MyElementType);
	}
	MuzzleArrow->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}


