// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "SPickUpActor.generated.h"

UCLASS()
class UESHOOTGAME_API ASPickUpActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere , Category = "Components")
	USphereComponent* SphereComp;
	
	UPROPERTY(VisibleAnywhere , Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly , Category = "PickupActor")
	TSubclassOf<ASPowerupActor> PowerUpClass;

	ASPowerupActor* PowerupInstance;

	UPROPERTY(EditInstanceOnly , Category = "PickupActor")
	float CooldownDuration;

	FTimerHandle TimerHandle_RespawnHandle;
	
	void ReSpawn();
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
