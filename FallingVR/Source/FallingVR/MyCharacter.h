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
	//苟造函数
	AMyCharacter();
	//右Trigger键按压和松开所调用的方法
	void TriggerRightPressed();
	void TriggerRightReleased();

	//左Trigger键按压和松开所调用的方法
	void TriggerLeftPressed();
	void TriggerLeftReleased();

	//右圆盘按压和松开所调用的方法
	void FaceButtonRightPressed();
	void FaceButtonRightReleased();

	//左圆盘按压和松开所调用的方法
	void FaceButtonLeftPressed();
	void FaceButtonLeftReleased();

	//设置是否允许开火
	//参数：bool
	void SetAllowFire(bool Choose);

	//获得当前手上的道具
	//参数：EMyHand Right代表右手，Left代表左手
	//返回类型AMyItemBase*，为空表示空手
	class AMyItemBase* GetMyItem(EMyHand::Type NewType);

	//设置右手的武器（右手只能拿武器）
	//参数：EMyWeapon 在MyEnums.h中查看
	//返回bool，true为设置成功，false为设置失败
	bool SetMyRightHandWeapon(EMyWeapon::Type NewType);

	//设置左手的道具（左手只能拿道具）
	//参数：EMyItem 在MyEnums.h中查看
	//返回bool，true为设置成功，false为设置失败
	bool SetMyLeftHandItem(EMyItem::Type NewType);

	//获得当前的武器类型
	//返回EMyWeapon 在MyEnums.h中查看
	EMyWeapon::Type GetCurrentWeaponType();

	//获得当前的道具类型
	//返回EMyItem 在MyEnums.h中查看
	EMyItem::Type GetCurrentItemType();

	bool GetTriggerState(EMyHand::Type NewType);

	bool GetRunningFireWeaponOn();
	
public:
	//Camera组件，也就是游戏的主视角
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* MyCamera;
	//右MotionController组件，右手控制器
	UPROPERTY(VisibleAnywhere)
		class UMotionControllerComponent* ControllerRight;
	//左MotionController组件，左手控制器
	UPROPERTY(VisibleAnywhere)
		class UMotionControllerComponent* ControllerLeft;
	//右Mesh组件，之后可能改成Skelemesh，右手的Mesh
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RightControllerMesh;
	//右Arrow组件，用于提供绑定的Transform信息
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* RightControllerAttachArrow;
	//右Mesh组件，之后可能改成Skelemesh，右手的Mesh
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftControllerMesh;
	//左Arrow组件，用于提供绑定的Transform信息
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* LeftControllerAttachArrow;

private:
	//用于存储右Trigger键的状态 true为按下，false为松开
	bool RightTriggerOn = false;
	//用于存储左Trigger键的状态 true为按下，false为松开
	bool LeftTriggerOn = false;
	//用于存储右脸键的状态 true为按下，false为松开
	bool FaceButtonRightOn = false;
	//用于存储左脸键的状态 true为按下，false为松开
	bool FaceButtonLeftOn = false;
	//枪的保险开关， true为允许射击， false为不允许射击
	bool AllowFire = true;
	bool RunningFireWeaponOn = false;

	//目前正在用的武器种类
	EMyWeapon::Type CurrentWeapon = EMyWeapon::Pistol;
	//目前正在选择的武器种类
	EMyWeapon::Type NowWeapon = EMyWeapon::Pistol;
	//目前正在使用的道具种类
	EMyItem::Type CurrentItem = EMyItem::FlashSphere;
	//目前正在选择的道具种类
	EMyItem::Type NowItem = EMyItem::FlashSphere;

	//右手的武器
	class AMyItemBase* MyRightHandItem;
	//左手的武器
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
