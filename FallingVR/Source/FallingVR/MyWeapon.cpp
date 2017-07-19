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

	ConstructorHelpers::FObjectFinder<UDataTable> TempWeaponData(TEXT("DataTable'/Game/Blueprint/DWeapon.DWeapon'"));
	if (TempWeaponData.Succeeded())
	{
		WeaponData = TempWeaponData.Object;
	}
	ConstructorHelpers::FObjectFinder<UDataTable> TempElementData(TEXT(""));
	if (TempElementData.Succeeded())
	{
		ElementData = TempElementData.Object;
	}
}

void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GetOwner()) && Cast<AMyCharacter>(GetOwner())->GetRunningFireWeaponOn() && MyDefaultProperty->RunningFire)
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
		MyDefaultProperty = WeaponData->FindRow<FWeaponType>(WeaponDataName[NewType], TEXT(""));

		WeaponMesh->SetSkeletalMesh(MyDefaultProperty->WeaponMesh);
		CDtime = MyDefaultProperty->FireCD;
		MuzzleArrow->SetRelativeLocation(MyDefaultProperty->MuzzleOffset);
		WeaponMesh->SetRelativeLocation(MyDefaultProperty->MeshOffset);
		TempBulletParam->BulletSize = MyDefaultProperty->BulletSize;
		TempBulletParam->MaxSpeed = MyDefaultProperty->Speed;
		TempBulletParam->LifeTime = MyDefaultProperty->LifeTime;
		TempBulletParam->BaseDamageValue = MyDefaultProperty->BaseDamageValue;
		TempBulletParam->ElementLevel = MyDefaultProperty->ElementLevel;
	}
}

void AMyWeapon::SetElementType(EMyElement::Type NewType)
{
	if (IsValid(ElementData))
	{
		MyElementType = NewType;
		TArray<FName> ElementName;
		ElementName = ElementData->GetRowNames();
		FElementStruct* TempElement = ElementData->FindRow<FElementStruct>(ElementName[NewType], TEXT(""));
		TempBulletParam->BulletMaterial = TempElement->BulletMaterial;
		TempBulletParam->BulletTraceParticle = TempElement->BulletTraceParticle;
		TempBulletParam->HitParticle = TempElement->HitActorParticle;
		TempBulletParam->ElementParticle = TempElement->ElementParticle;
		TempBulletParam->ExplosionParticle = TempElement->ExplosionParticle;
	}
}

void AMyWeapon::ItemFunction()
{
	//设置粒子特效和射线碰撞的参数
	FHitResult TraceHit;
	FVector LineStart = MuzzleArrow->GetComponentLocation();
	FVector LineEnd = LineStart + MuzzleArrow->GetForwardVector() * MyDefaultProperty->FireRange;
	FTransform MuzzleTransform = MuzzleArrow->GetComponentTransform();
	UWorld* MyWorld = GetWorld();
	//生成枪口栗子特效
	if (MyDefaultProperty->MuzzleParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(MyWorld, MyDefaultProperty->MuzzleParticle, MuzzleTransform, true);
	}

	//根据武器种类执行相应的子弹生成方式
	if (MyWeaponType == EMyWeapon::Shotgun)
	{
		BulletIsMultiProjectle();
	}
	else
	{
		BulletIsSingleProjectle();
	}
}

void AMyWeapon::BulletIsSingleProjectle()
{
	UWorld* MyWorld = GetWorld();

	AddRecoil();

	AMyBulletBase* TempBullet = MyWorld->SpawnActor<AMyBulletBase>(MuzzleArrow->GetComponentLocation(), MuzzleArrow->GetComponentRotation());
	TempBullet->BeginInit(*TempBulletParam, GetOwner(), MyWeaponType, GetOwner()->MyElementType);
}

void AMyWeapon::BulletIsMultiProjectle()
{
	UWorld* MyWorld = GetWorld();

	for (int i = 0; i < ShotGunBulletNum; i++)
	{
		MuzzleArrow->SetRelativeRotation(FRotator(FMath::RandRange(-10.f, 10.f), FMath::RandRange(-10.f, 10.f), FMath::RandRange(-10.f, 10.f)));
		AMyBulletBase* TempBullet = MyWorld->SpawnActor<AMyBulletBase>(MuzzleArrow->GetComponentLocation(), MuzzleArrow->GetComponentRotation());
		TempBullet->BeginInit(*TempBulletParam, GetOwner(), MyWeaponType, GetOwner()->MyElementType);
	}
	MuzzleArrow->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

bool AMyWeapon::GetWeaponOneHanded()
{
	return MyDefaultProperty->One_Handed;
}

FVector2D AMyWeapon::GetTwoAllowRange()
{
	return FVector2D(MyDefaultProperty->TwoHandMinDistance, MyDefaultProperty->TwoHandMaxDistance);
}

bool AMyWeapon::isRunningFire()
{
	return MyDefaultProperty->RunningFire;
}

void AMyWeapon::AddRecoil()
{
	GetWorldTimerManager().SetTimer(RecoilHandle, this, &AMyWeapon::SetRecoilRestore, MyDefaultProperty->RecoilRestoreTime);
	FRotator AddValue = FRotator(FMath::RandRange(-MyDefaultProperty->RecoilValueK, MyDefaultProperty->RecoilValueK), 0.f, FMath::RandRange(-MyDefaultProperty->RecoilValueK, MyDefaultProperty->RecoilValueK));
	FRotator OringinRotation = FRotator(WeaponMesh->GetRelativeTransform().GetRotation().Euler().X, WeaponMesh->GetRelativeTransform().GetRotation().Euler().Y, WeaponMesh->GetRelativeTransform().GetRotation().Euler().Z);
	WeaponMesh->SetRelativeRotation(OringinRotation + AddValue);
}

void AMyWeapon::SetRecoilRestore()
{
	bRecoil = false;
	WeaponMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}