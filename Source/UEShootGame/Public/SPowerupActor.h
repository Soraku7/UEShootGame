// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class UESHOOTGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPowerupActor();

	UPROPERTY(EditDefaultsOnly , Category = "Powerups")
	float PowerupInterval;
	
	UPROPERTY(EditDefaultsOnly , Category = "Powerups")
	int32 TotalNrOfTicks;

	FTimerHandle TimerHandle_PowerupTick;

	int32 TickProcessed;
	
	UFUNCTION()
	void OnTickPowerUp();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void ActivePowerup();
	
	UFUNCTION(BlueprintImplementableEvent , Category = "Powerups")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent , Category = "Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent , Category = "Powerups")
	void OnExpire();
	
};
