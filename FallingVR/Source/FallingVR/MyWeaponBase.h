// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeaponBase.generated.h"

UCLASS()
class FALLINGVR_API AMyWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeaponBase();

public:

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* WeaponMesh;
	UPROPERTY()
		class UArrowComponent* MuzzleArrow;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* HandArrow1;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* HandArrow2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		class UParticleSystem* MuzzleParticle = NULL;
	//射击频率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		float FireRate = 1.f;
	//射击最大距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		float FireDistance = 1000.f;
	//射击基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		float BaseDamageValue = 10.f;
	//是否为单手武器
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		bool One_handed = true;

private:
	//武器能发射吗？（CD）
	bool CanFire = true;
	//武器允许使用吗？
	bool AllowFire = true;
	FTimerHandle FireRateTimeHandle;
	//武器拥有者
	class AMyCharacterBase* Owner;

private:


	void InitCanFire();
	void SetAllowFire(bool Choose);

	void Fire();
	void AddDamage(float DamageValue, FHitResult& HitResult, AMyCharacterBase* Causer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
