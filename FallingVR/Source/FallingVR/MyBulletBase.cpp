// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBulletBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyCharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyBulletBase::AMyBulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicateMovement = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->Velocity = FVector(1000.f, 0.f, 0.f);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->AttachTo(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	CollisionMesh->AttachTo(RootComponent);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionMesh->SetCollisionObjectType(ECC_EngineTraceChannel1);
	CollisionMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AMyBulletBase::CollisionBeginOverlap);
}

// Called when the game starts or when spawned
void AMyBulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBulletBase::BeginInit(AMyCharacterBase* Owner_, EMyWeapon::Type NewWeaponType, EMyElement::Type NewElementType)
{
	if (IsValid(Owner_))
	{
		Owner = Owner_;
	}

	MyWeaponType = NewWeaponType;
	MyElementType = NewElementType;
	UWorld* MyWorld = GetWorld();
	if (IsValid(MyWorld))
	{
		UGameplayStatics::SpawnEmitterAttached(SelfParticle, BulletMesh);
	}
}

void AMyBulletBase::CollisionBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherCompnent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& Hit)
{
	UWorld* MyWorld = GetWorld();
	FVector HitLocation = Hit.ImpactPoint;
	FRotator HitRotation = Hit.ImpactNormal.Rotation();
	FVector DefaultSize = FVector(1.f, 1.f, 1.f);
	FTransform HitNormalTransform = FTransform(HitRotation, HitLocation, DefaultSize);
	FTransform HitTransform = FTransform(FRotator(0.f, 0.f, 0.f), HitLocation, DefaultSize);

	if (IsValid(Owner))
	{
		if (OtherActor != Owner)
		{
			if (AMyCharacterBase* HitCharacter = Cast<AMyCharacterBase>(OtherActor))
			{
				HitCharacter->ApplyDamage(BaseDamage, Owner, Hit);
			}
			if (IsValid(MyWorld))
			{
				UGameplayStatics::SpawnEmitterAtLocation(MyWorld, HitParticle, HitNormalTransform, true);
				BulletFunction();
			}
		}
	}
}

void AMyBulletBase::BulletFunction()
{

}

void AMyBulletBase::DestroyBullet(FVector HitLocation)
{
	FTransform HitTransform(FRotator(0.f, 0.f, 0.f), HitLocation, FVector(1.f, 1.f, 1.f));
	UWorld* MyWorld = GetWorld();
	if (IsValid(MyWorld))
	{
		UGameplayStatics::SpawnEmitterAtLocation(MyWorld, DestroyParticle, HitTransform, true);
	}
	Destroy(true);
}

