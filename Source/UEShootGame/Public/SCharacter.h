// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHealthComponent.h"
#include "Sweapon.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
UCLASS()
class UESHOOTGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	UPROPERTY(VisibleAnywhere , Blueprintable , Category = "Components")
	UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere , Blueprintable , Category = "Components")
	USpringArmComponent* SpringArmComp;
	
	USHealthComponent* HealthComp;
	//对焦
	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly , Category = "Player")
	//放大视场
	float ZoomedFOV;

	//默认视场
	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly , Category = "Player" , meta = (ClampMin = 0.1 , ClampMax = 100.0))
	float ZoomInterSpeed;

	void BeginZoom();
	void EndZoom();
	
	UPROPERTY(EditDefaultsOnly , Category = "Player")
	TSubclassOf<ASweapon> StartWeaponClass;

	UPROPERTY(VisibleDefaultsOnly , Category = "Player")
	FName WeaponAttachSocketName;
	
	void StartFire();
	void StopFire();
	
	/**
	 * 玩家是否死亡
	 * @param HealthComponent 
	 * @param Health 
	 * @param HealthDelta 
	 * @param DamageType 
	 * @param InstigatedBy 
	 * @param DamageCauser 
	 */
	UFUNCTION()
	void OnHealthChanged(USHealthComponent*  OwningHealthComp
	, float  Health , float  HealthDelta , const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated , BlueprintReadOnly , Category = "Player")
	bool bDied;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(Replicated)
	ASweapon* CurrentWeapon;

};
