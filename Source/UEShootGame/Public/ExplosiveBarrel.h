﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHealthComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class UESHOOTGAME_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere , Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere , Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere , Category = "Components")
	URadialForceComponent* RadialForceComp;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent*  HealthComponent
	, float  Health , float  HealthDelta , const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
	bool bExploded;

	UFUNCTION()
	void OnRep_Exploded();

	UPROPERTY(EditDefaultsOnly , Category = "FX")
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly , Category = "FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly , Category = "FX")
	UMaterialInterface* ExplodedMaterial;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
