﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHealthComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"
#include "STrackerBot.generated.h"


UCLASS()
class UESHOOTGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly , Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly , Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly , Category = "Components")
	USphereComponent* SphereComp;

	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	FVector GetNextPathPoint();

	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	float MovementForce;
	
	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	float RequireDistanceToTarget;
	
	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	bool bUseVelocityChange;

	UMaterialInstanceDynamic* MatIns;

	void SelfDestruct();

	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	bool bExploded;

	bool bStartSelfDestruection;

	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	float SelfDamageInterval;

	FTimerHandle TimerHandle_SelfDamage;

	void DamageSelf();

	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly , Category = "TrackerBot")
	USoundCue* ExplodeSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
