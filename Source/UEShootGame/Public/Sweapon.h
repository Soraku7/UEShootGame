// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/EnumAsByte.h"
#include "Sweapon.generated.h"

//单次武器射击轨迹信息
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY();

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	UPROPERTY()
	FVector_NetQuantize TraceTo;
};
UCLASS()
class UESHOOTGAME_API ASweapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASweapon();

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Components")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere , Blueprintable , Category = "Components")
	USkeletalMeshComponent* MeshComp;
	
	void PlayFireEffect(FVector TraceEnd);

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);
	
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	//击中身体特效
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	//击中物体特效
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly , Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCamShake;

	UPROPERTY(EditDefaultsOnly , Category = "Weapon")
	float BaseDamage;

	//射击间隔时间
	FTimerHandle TimerHandle_TimeBetweenShots;

	//最后一次射击时间
	float LastFireTime;

	//武器每分钟发射子弹数量
	UPROPERTY(EditDefaultsOnly , Category = "Weapon")
	float RateOfFire;

	//攻击间隔
	float TimeBetweenShots;

	//服务器改变时 在客户端进行同步
	//网络射击信息  当结构体内容改变自动调用网络复制函数
	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	//网络复制函数
	UFUNCTION()
	void OnRep_HitScanTrace();

public:
	UFUNCTION(BlueprintCallable , Category = "Weapon")
	virtual void Fire();

	UFUNCTION(Server , Reliable , WithValidation)
	void ServerFire();
	
	void StartFire();
	
	void StopFire();
};
