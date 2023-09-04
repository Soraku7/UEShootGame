// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sweapon.generated.h"

UCLASS()
class UESHOOTGAME_API ASweapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASweapon();

protected:

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Components")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere , Blueprintable , Category = "Components")
	USkeletalMeshComponent* MeshComp;
	
	void PlayFireEffect(FVector TraceEnd);
	
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* TracerEffect;

	
public:
	UFUNCTION(BlueprintCallable , Category = "Weapon")
	virtual void Fire();
};
