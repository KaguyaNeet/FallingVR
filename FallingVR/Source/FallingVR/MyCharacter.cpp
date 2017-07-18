// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "MotionControllerComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyItemBase.h"
#include "MyWeapon.h"


AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//设置默认的胶囊碰撞体（根组件）的碰撞类型为无碰撞
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//创建Camera组件并绑定于根组件
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	MyCamera->AttachTo(RootComponent);

	//创建左右控制器
	//将左右控制器绑定于根组件
	//创建左右手的Mesh
	//将左右手Mesh绑定于相应的控制器
	//设置左右手的Mesh为无碰撞
	//创建左右手的绑定提示箭头
	//将左右箭头绑定于Mesh组件
	ControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	ControllerRight->AttachTo(RootComponent);
	ControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	ControllerLeft->AttachTo(RootComponent);

	RightControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightControllerMesh"));
	RightControllerMesh->AttachTo(ControllerRight);
	RightControllerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftControllerMesh"));
	LeftControllerMesh->AttachTo(ControllerLeft);
	LeftControllerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightControllerAttachArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightControllerAttachArrow"));
	RightControllerAttachArrow->AttachTo(RightControllerMesh);
	RightControllerAttachArrow->SetRelativeRotation(FRotator(0.f, -50.f, 0.f));
	LeftControllerAttachArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftControllerAttachArrow"));
	LeftControllerAttachArrow->AttachTo(LeftControllerMesh);
}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetMyRightHandWeapon(EMyWeapon::Shotgun);
}


void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TriggerLeft", IE_Pressed, this, &AMyCharacter::TriggerLeftPressed);
	PlayerInputComponent->BindAction("TriggerLeft", IE_Released, this, &AMyCharacter::TriggerLeftReleased);
	PlayerInputComponent->BindAction("TriggerRight", IE_Pressed, this, &AMyCharacter::TriggerRightPressed);
	PlayerInputComponent->BindAction("TriggerRight", IE_Released, this, &AMyCharacter::TriggerRightReleased);

	PlayerInputComponent->BindAction("FaceButtonRight", IE_Pressed, this, &AMyCharacter::FaceButtonRightPressed);
	PlayerInputComponent->BindAction("FaceButtonRight", IE_Released, this, &AMyCharacter::FaceButtonRightReleased);
	PlayerInputComponent->BindAction("FaceButtonLeft", IE_Pressed, this, &AMyCharacter::FaceButtonLeftPressed);
	PlayerInputComponent->BindAction("FaceButtonLeft", IE_Released, this, &AMyCharacter::FaceButtonLeftReleased);
}

void AMyCharacter::TriggerLeftPressed()
{
	LeftTriggerOn = true;
	if (IsValid(MyLeftHandItem))
	{
		MyLeftHandItem->UseItem();
	}

}
void AMyCharacter::TriggerLeftReleased()
{
	LeftTriggerOn = false;
	MyLeftHandItem->PauseFunction();
}

void AMyCharacter::TriggerRightPressed()
{
	RightTriggerOn = true;
	if (IsValid(MyRightHandItem))
	{
		AMyWeapon* MyRightHandWeapon = Cast<AMyWeapon>(MyRightHandItem);
		if (MyRightHandWeapon->One_handed)
		{
			if (MyRightHandWeapon->RunningFire)
			{
				RunningFireWeaponOn = true;
			}
			else
			{
				MyRightHandWeapon->UseItem();
			}
		}
		else
		{
			if (LeftTriggerOn)
			{
				if (GetTwoMotionControllerDistance() < MyRightHandWeapon->TwoHandMaxDistance && GetTwoMotionControllerDistance() > MyRightHandWeapon->TwoHandMinDistance)
				{
					RunningFireWeaponOn = true;
				}
			}
		}
	}
}
void AMyCharacter::TriggerRightReleased()
{
	RightTriggerOn = false;
	RunningFireWeaponOn = false;
}

void AMyCharacter::FaceButtonRightPressed()
{
	FaceButtonRightOn = true;
}
void AMyCharacter::FaceButtonRightReleased()
{
	FaceButtonRightOn = false;
	SetMyRightHandWeapon(NowWeapon);
}

void AMyCharacter::FaceButtonLeftPressed()
{
	FaceButtonLeftOn = true;
}
void AMyCharacter::FaceButtonLeftReleased()
{
	FaceButtonLeftOn = false;
	SetMyLeftHandItem(NowItem);
}

float AMyCharacter::GetTwoMotionControllerDistance()
{
	return FMath::Abs((ControllerRight->GetComponentLocation() - ControllerLeft->GetComponentLocation()).Size());
}

void AMyCharacter::SetAllowFire(bool Choose)
{
	AllowFire = Choose;
}

AMyItemBase* AMyCharacter::GetMyItem(EMyHand::Type NewType)
{
	switch (NewType)
	{
	case EMyHand::LeftHand: if (MyLeftHandItem)
	{
		return MyLeftHandItem;
	}
							else
							{
								return NULL;
							}
							break;

	case EMyHand::RightHand: if (MyRightHandItem)
	{
		return MyRightHandItem;
	}
							 else
							 {
								 return NULL;
							 }
							 break;
	}
	return NULL;
}

//设置右手的武器（右手只能拿武器）
//参数：EMyWeapon 在MyEnums.h中查看
//返回bool，true为设置成功，false为设置失败
bool AMyCharacter::SetMyRightHandWeapon(EMyWeapon::Type NewType)
{
	//将当前使用的武器类型变为NewType
	CurrentWeapon = NewType;
	UWorld* MyWorld = GetWorld();
	//武器的生成Transform参数
	FVector RightHandLocation = RightControllerAttachArrow->GetComponentLocation();
	FRotator RightHandRotation = RightControllerAttachArrow->GetComponentRotation();

	if (MyWorld)
	{
		AMyItemBase* RightHandItem = MyWorld->SpawnActor<AMyWeapon>(RightHandLocation, RightHandRotation);
		//将武器绑定于箭头上
		RightHandItem->AttachToComponent(RightControllerAttachArrow, FAttachmentTransformRules::KeepRelativeTransform);
		RightHandItem->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
		RightHandItem->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
		if (IsValid(MyRightHandItem))
		{
			MyRightHandItem->DestroyItem();
		}
		MyRightHandItem = RightHandItem;
		//执行武器的初始化
		Cast<AMyWeapon>(MyRightHandItem)->InitItem(this, NewType);
		return true;
	}
	else
	{
		return false;
	}
}

//设置左手的道具（左手只能拿道具）
//参数：EMyItem 在MyEnums.h中查看
//返回bool，true为设置成功，false为设置失败
bool AMyCharacter::SetMyLeftHandItem(EMyItem::Type NewType)
{
	UWorld* MyWorld = GetWorld();
	FVector LeftHandLocation = LeftControllerAttachArrow->GetComponentLocation();
	FRotator LeftHandRotation = LeftControllerAttachArrow->GetComponentRotation();

	if (MyWorld)
	{
		AMyItemBase* LeftHandItem = MyWorld->SpawnActor<AMyItemBase>(LeftHandLocation, LeftHandRotation);
		LeftHandItem->AttachToComponent(LeftControllerAttachArrow, FAttachmentTransformRules::KeepRelativeTransform);
		LeftHandItem->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
		LeftHandItem->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
		if (MyLeftHandItem)
		{
			MyLeftHandItem->DestroyItem();
		}
		MyLeftHandItem = LeftHandItem;
		MyLeftHandItem->InitItem(this);
		return true;
	}
	else
	{
		return false;
	}
}

EMyWeapon::Type AMyCharacter::GetCurrentWeaponType()
{
	return CurrentWeapon;
}
EMyItem::Type AMyCharacter::GetCurrentItemType()
{
	return CurrentItem;
}

bool AMyCharacter::GetTriggerState(EMyHand::Type NewType)
{
	if (NewType == EMyHand::RightHand)
	{
		return RightTriggerOn;
	}
	else
	{
		return LeftTriggerOn;
	}
}

bool AMyCharacter::GetRunningFireWeaponOn()
{
	return RunningFireWeaponOn;
}