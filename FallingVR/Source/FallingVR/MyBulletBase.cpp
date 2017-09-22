// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBulletBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "MyStructs.h"

// Sets default values
AMyBulletBase::AMyBulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempCube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(TEXT("Material'/Game/Material/M_Bullet.M_Bullet'"));
	ConstructorHelpers::FObjectFinder<UDataTable> TempData(TEXT("DataTable'/Game/Blueprint/DWeapon.DWeapon'"));

	if (TempData.Succeeded())
	{
		WeaponData = TempData.Object;
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->Velocity = FVector(1000.f, 0.f, 0.f);
	MovementComponent->InitialSpeed = 3000.f;
	MovementComponent->MaxSpeed = 3000.f;
	MovementComponent->ProjectileGravityScale = 0.f;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->AttachTo(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletMesh->SetStaticMesh(TempCube.Object);
	BulletMesh->SetMaterial(0, TempMaterial.Object);
	BulletMesh->SetWorldScale3D(FVector(1.f, 0.1f, 0.1f));
	BulletMesh->SetEnableGravity(false);

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	CollisionMesh->AttachTo(RootComponent);
	CollisionMesh->SetEnableGravity(false);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionMesh->SetCollisionObjectType(ECC_EngineTraceChannel1);
	CollisionMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	//CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AMyBulletBase::CollisionBeginOverlap);
	CollisionMesh->OnComponentHit.AddDynamic(this, &AMyBulletBase::CollisionHit);

	
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

void AMyBulletBase::BeginInit(const FBulletParam& BulletParam, AMyCharacterBase* Owner_, EMyWeapon::Type NewWeaponType, EMyElement::Type NewElementType)
{
	MovementComponent->MaxSpeed = BulletParam.MaxSpeed;
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AMyBulletBase::DestroySelf, BulletParam.LifeTime);
	BulletMesh->SetWorldScale3D(BulletParam.BulletSize);
	BulletMesh->SetMaterial(0, BulletParam.BulletMaterial);
	HitParticle = BulletParam.HitParticle;
	DestroyParticle = BulletParam.ExplosionParticle;
	BaseDamage = BulletParam.BaseDamageValue;
	ElementLevel = BulletParam.ElementLevel;

	if (IsValid(Owner_))
	{
		Owner = Owner_;
	}

	MyWeaponType = NewWeaponType;
	MyElementType = NewElementType;


	UWorld* MyWorld = GetWorld();
	if (IsValid(MyWorld))
	{
		UGameplayStatics::SpawnEmitterAttached(BulletParam.ElementParticle, BulletMesh);
		UGameplayStatics::SpawnEmitterAttached(BulletParam.BulletTraceParticle, BulletMesh);
	}
}

//void AMyBulletBase::CollisionBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherCompnent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& Hit)
//{
//	UWorld* MyWorld = GetWorld();
//	FVector HitLocation = Hit.ImpactPoint;
//	FRotator HitRotation = Hit.ImpactNormal.Rotation();
//	FVector DefaultSize = FVector(1.f, 1.f, 1.f);
//	FTransform HitNormalTransform = FTransform(HitRotation, HitLocation, DefaultSize);
//	FTransform HitTransform = FTransform(FRotator(0.f, 0.f, 0.f), HitLocation, DefaultSize);
//
//	if (IsValid(Owner))
//	{
//		if (OtherActor != Owner)
//		{
//			if (AMyCharacterBase* HitCharacter = Cast<AMyCharacterBase>(OtherActor))
//			{
//				HitCharacter->ApplyDamage(MyElementType, ElementLevel, BaseDamage, Owner, Hit);
//			}
//			if (IsValid(MyWorld))
//			{
//				UGameplayStatics::SpawnEmitterAtLocation(MyWorld, HitParticle, HitNormalTransform, true);
//				BulletFunction(Hit.ImpactPoint);
//			}
//		}
//	}
//}

void AMyBulletBase::CollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UWorld* MyWorld = GetWorld();
	FVector HitLocation = Hit.Location;
	FRotator HitRotation = NormalImpulse.Rotation();
	FVector DefaultSize = FVector(1.f, 1.f, 1.f);
	FTransform HitNormalTransform = FTransform(HitRotation, HitLocation, DefaultSize);
	FTransform HitTransform = FTransform(FRotator(0.f, 0.f, 0.f), HitLocation, DefaultSize);

	if (IsValid(Owner))
	{
		if (OtherActor != Owner)
		{
			if (AMyCharacterBase* HitCharacter = Cast<AMyCharacterBase>(OtherActor))
			{
				HitCharacter->ApplyDamage(MyElementType, ElementLevel, BaseDamage, Owner, Hit);
			}
			if (IsValid(MyWorld))
			{
				UGameplayStatics::SpawnEmitterAtLocation(MyWorld, HitParticle, HitNormalTransform, true);
				BulletFunction(Hit.Location);
			}
		}
	}
}

void AMyBulletBase::BulletFunction(FVector HitLocation)
{
	UWorld* MyWorld = GetWorld();
	FTransform HitTransform = FTransform(FRotator(0.f, 0.f, 0.f), HitLocation, FVector(1.f, 1.f, 1.f));
	if (IsValid(MyWorld))
	{
		if (MyElementType == EMyElement::Explosion)
		{
			UGameplayStatics::SpawnEmitterAtLocation(MyWorld, DestroyParticle, HitTransform, true);
			TArray<AActor*> IgnoreActor;
			UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage * ElementLevel, HitLocation, 100.f * ElementLevel, NULL, IgnoreActor, Owner);
			return;
		}
		if (MyElementType == EMyElement::Ice)
		{
			UGameplayStatics::SpawnEmitterAtLocation(MyWorld, DestroyParticle, HitTransform, true);
		}
	}
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

void AMyBulletBase::DestroySelf()
{
	GetWorldTimerManager().ClearTimer(DestroyHandle);
	Destroy();
}

