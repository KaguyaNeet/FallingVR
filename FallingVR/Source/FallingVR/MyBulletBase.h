// Fill out your copyright notice in the Description page of Project Settings.
#include "MyEnums.h"
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBulletBase.generated.h"

UCLASS()
class FALLINGVR_API AMyBulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBulletBase();

	void BeginInit(class AMyCharacterBase* Owner_, EMyWeapon::Type NewWeaponType, EMyElement::Type NewElementType);

	void DestroyBullet(FVector HitLocation);

public:

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* BulletMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* CollisionMesh;

private:
	UFUNCTION()
		void CollisionBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherCompnent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& Hit);

	void BulletFunction();
private:

	class AMyCharacterBase* Owner = NULL;

	float BaseDamage = 10.f;

	UParticleSystem* HitParticle = NULL;
	UParticleSystem* DestroyParticle = NULL;
	UParticleSystem* SelfParticle = NULL;

	EMyElement::Type MyElementType = EMyElement::None;
	EMyWeapon::Type MyWeaponType = EMyWeapon::Pistol;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
