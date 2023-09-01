// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sweapon.h"
#include "SProjectileWeapon.generated.h"

UCLASS()
class UESHOOTGAME_API ASProjectileWeapon : public ASweapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASProjectileWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly , Category = "ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
