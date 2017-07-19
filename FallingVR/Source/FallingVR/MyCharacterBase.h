// Fill out your copyright notice in the Description page of Project Settings.
#include "MyEnums.h"
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacterBase.generated.h"

UCLASS()
class FALLINGVR_API AMyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacterBase();
	//ִ���˺�
	virtual void ApplyDamage(EMyElement::Type DamageType, float ElementLevel, float DamageValue, AMyCharacterBase* Causer, const FHitResult& HitResult);
	//ִ������
	virtual void ApplyDeath(AMyCharacterBase* Causer);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ApplyElementDamage(EMyElement::Type DamageType, float ParamElementLevel);

public:
	//�������ֵ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		float MaxLifeValue = 100.f;
	//��ǰ����ֵ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default Property")
		float LifeValue = 100.f;
	//������ʱ��������Ч
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		UParticleSystem* FireParticle;

	EMyElement::Type MyElementType = EMyElement::None;
	
};
