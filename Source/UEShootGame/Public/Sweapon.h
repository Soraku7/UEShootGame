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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Components")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere , Blueprintable , Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UFUNCTION(BlueprintCallable , Category = "Weapon")
	void Fire();

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Weapon")
	UParticleSystem* ImpactEffect;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
