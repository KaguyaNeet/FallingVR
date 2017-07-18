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

	//����Ĭ�ϵĽ�����ײ�壨�����������ײ����Ϊ����ײ
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//����Camera��������ڸ����
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	MyCamera->AttachTo(RootComponent);

	//�������ҿ�����
	//�����ҿ��������ڸ����
	//���������ֵ�Mesh
	//��������Mesh������Ӧ�Ŀ�����
	//���������ֵ�MeshΪ����ײ
	//���������ֵİ���ʾ��ͷ
	//�����Ҽ�ͷ����Mesh���
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

//�������ֵ�����������ֻ����������
//������EMyWeapon ��MyEnums.h�в鿴
//����bool��trueΪ���óɹ���falseΪ����ʧ��
bool AMyCharacter::SetMyRightHandWeapon(EMyWeapon::Type NewType)
{
	//����ǰʹ�õ��������ͱ�ΪNewType
	CurrentWeapon = NewType;
	UWorld* MyWorld = GetWorld();
	//����������Transform����
	FVector RightHandLocation = RightControllerAttachArrow->GetComponentLocation();
	FRotator RightHandRotation = RightControllerAttachArrow->GetComponentRotation();

	if (MyWorld)
	{
		AMyItemBase* RightHandItem = MyWorld->SpawnActor<AMyWeapon>(RightHandLocation, RightHandRotation);
		//���������ڼ�ͷ��
		RightHandItem->AttachToComponent(RightControllerAttachArrow, FAttachmentTransformRules::KeepRelativeTransform);
		RightHandItem->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
		RightHandItem->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
		if (IsValid(MyRightHandItem))
		{
			MyRightHandItem->DestroyItem();
		}
		MyRightHandItem = RightHandItem;
		//ִ�������ĳ�ʼ��
		Cast<AMyWeapon>(MyRightHandItem)->InitItem(this, NewType);
		return true;
	}
	else
	{
		return false;
	}
}

//�������ֵĵ��ߣ�����ֻ���õ��ߣ�
//������EMyItem ��MyEnums.h�в鿴
//����bool��trueΪ���óɹ���falseΪ����ʧ��
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