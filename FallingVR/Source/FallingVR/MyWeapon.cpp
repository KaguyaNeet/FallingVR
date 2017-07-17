// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWeapon.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacterBase.h"

AMyWeapon::AMyWeapon()
{
	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));
	MuzzleArrow->AttachTo(ItemMesh);

	HandArrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow1"));
	HandArrow1->AttachTo(ItemMesh);

	HandArrow2 = CreateDefaultSubobject<UArrowComponent>(TEXT("HandArrow2"));
	HandArrow2->AttachTo(ItemMesh);

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
	//����������ײ�ж�
	if (MyWorld->LineTraceSingleByChannel(TraceHit, LineStart, LineEnd, ECollisionChannel::ECC_WorldDynamic))
	{
		AddDamage(BaseDamageValue, TraceHit, GetOwner());
	}
}

void AMyWeapon::AddDamage(float DamageValue, FHitResult& HitResult, AMyCharacterBase* Causer)
{
	if (AMyCharacterBase* HitCharacter = Cast<AMyCharacterBase>(HitResult.GetActor()))
	{
		HitCharacter->ApplyDamage(DamageValue, Causer, HitResult);
	}
}


