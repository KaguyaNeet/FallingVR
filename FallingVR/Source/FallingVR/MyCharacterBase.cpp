// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterBase.h"
#include "Kismet/GameplayStatics.h"


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

//ִ���˺�
void AMyCharacterBase::ApplyDamage(float DamageValue, AMyCharacterBase* Causer, FHitResult& HitResult)
{
	//�����˺�
	LifeValue -= DamageValue;

	//��������������Ч��Transform
	UWorld* MyWorld = GetWorld();
	FVector DamageLocation = HitResult.ImpactPoint;
	FVector DamageNormal = HitResult.ImpactNormal;
	FRotator DamageRotation = DamageNormal.Rotation();
	FTransform DamageTransform = FTransform(DamageRotation, DamageLocation, FVector(1.f, 1.f, 1.f));

	//���ɻ��������������Ч
	if (MyWorld)
	{
		UGameplayStatics::SpawnEmitterAtLocation(MyWorld, UnderAttackParticle, DamageTransform, true);
	}

	//�ж��Ƿ�����
	if (LifeValue <= 0)
	{
		ApplyDeath(Causer);
	}
}

//ִ����������_����
void AMyCharacterBase::ApplyDeath(AMyCharacterBase* Causer)
{

}

