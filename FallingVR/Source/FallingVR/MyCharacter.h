// Fill out your copyright notice in the Description page of Project Settings.
#include "MyEnums.h"
#pragma once

#include "CoreMinimal.h"
#include "MyCharacterBase.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGVR_API AMyCharacter : public AMyCharacterBase
{
	GENERATED_BODY()
	
public:
	//���캯��
	AMyCharacter();
	//��Trigger����ѹ���ɿ������õķ���
	void TriggerRightPressed();
	void TriggerRightReleased();

	//��Trigger����ѹ���ɿ������õķ���
	void TriggerLeftPressed();
	void TriggerLeftReleased();

	//��Բ�̰�ѹ���ɿ������õķ���
	void FaceButtonRightPressed();
	void FaceButtonRightReleased();

	//��Բ�̰�ѹ���ɿ������õķ���
	void FaceButtonLeftPressed();
	void FaceButtonLeftReleased();

	//�����Ƿ�������
	//������bool
	void SetAllowFire(bool Choose);

	//��õ�ǰ���ϵĵ���
	//������EMyHand Right�������֣�Left��������
	//��������AMyItemBase*��Ϊ�ձ�ʾ����
	class AMyItemBase* GetMyItem(EMyHand::Type NewType);

	//�������ֵ�����������ֻ����������
	//������EMyWeapon ��MyEnums.h�в鿴
	//����bool��trueΪ���óɹ���falseΪ����ʧ��
	bool SetMyRightHandWeapon(EMyWeapon::Type NewType);

	//�������ֵĵ��ߣ�����ֻ���õ��ߣ�
	//������EMyItem ��MyEnums.h�в鿴
	//����bool��trueΪ���óɹ���falseΪ����ʧ��
	bool SetMyLeftHandItem(EMyItem::Type NewType);

	//��õ�ǰ����������
	//����EMyWeapon ��MyEnums.h�в鿴
	EMyWeapon::Type GetCurrentWeaponType();

	//��õ�ǰ�ĵ�������
	//����EMyItem ��MyEnums.h�в鿴
	EMyItem::Type GetCurrentItemType();

	bool GetTriggerState(EMyHand::Type NewType);

	bool GetRunningFireWeaponOn();
	
public:
	//Camera�����Ҳ������Ϸ�����ӽ�
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* MyCamera;
	//��MotionController��������ֿ�����
	UPROPERTY(VisibleAnywhere)
		class UMotionControllerComponent* ControllerRight;
	//��MotionController��������ֿ�����
	UPROPERTY(VisibleAnywhere)
		class UMotionControllerComponent* ControllerLeft;
	//��Mesh�����֮����ܸĳ�Skelemesh�����ֵ�Mesh
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RightControllerMesh;
	//��Arrow����������ṩ�󶨵�Transform��Ϣ
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* RightControllerAttachArrow;
	//��Mesh�����֮����ܸĳ�Skelemesh�����ֵ�Mesh
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftControllerMesh;
	//��Arrow����������ṩ�󶨵�Transform��Ϣ
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* LeftControllerAttachArrow;

private:
	//���ڴ洢��Trigger����״̬ trueΪ���£�falseΪ�ɿ�
	bool RightTriggerOn = false;
	//���ڴ洢��Trigger����״̬ trueΪ���£�falseΪ�ɿ�
	bool LeftTriggerOn = false;
	//���ڴ洢��������״̬ trueΪ���£�falseΪ�ɿ�
	bool FaceButtonRightOn = false;
	//���ڴ洢��������״̬ trueΪ���£�falseΪ�ɿ�
	bool FaceButtonLeftOn = false;
	//ǹ�ı��տ��أ� trueΪ��������� falseΪ���������
	bool AllowFire = true;
	bool RunningFireWeaponOn = false;

	//Ŀǰ�����õ���������
	EMyWeapon::Type CurrentWeapon = EMyWeapon::Pistol;
	//Ŀǰ����ѡ�����������
	EMyWeapon::Type NowWeapon = EMyWeapon::Pistol;
	//Ŀǰ����ʹ�õĵ�������
	EMyItem::Type CurrentItem = EMyItem::FlashSphere;
	//Ŀǰ����ѡ��ĵ�������
	EMyItem::Type NowItem = EMyItem::FlashSphere;

	//���ֵ�����
	class AMyItemBase* MyRightHandItem;
	//���ֵ�����
	class AMyItemBase* MyLeftHandItem;

private:

	float GetTwoMotionControllerDistance();
	bool IsTwoHandAllow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
