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
	
	void ActivatePowerup(AActor* ActiveFor);
	
	UFUNCTION(BlueprintImplementableEvent , Category = "Powerups")
	void OnActivated(AActor* ActiveFor);

	UFUNCTION(BlueprintImplementableEvent , Category = "Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent , Category = "Powerups")
	void OnExpire();

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
	bool bIsPowerupActive;

	UFUNCTION()
	void OnRep_PowerupActive();
	
	UFUNCTION(BlueprintImplementableEvent , Category = "Powerups")
	void OnPowerupStateChange(bool bNewIsActive);
};
