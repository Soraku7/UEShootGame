// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNrOfTicks = 0;
}

void ASPowerupActor::OnTickPowerUp()
{
	TickProcessed ++;

	OnPowerupTicked();
	
	if(TickProcessed > TotalNrOfTicks)
	{
		OnExpire();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	

}

void ASPowerupActor::ActivePowerup()
{
	OnActivated();
	
	if(PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick , this , &ASPowerupActor::OnTickPowerUp ,
	PowerupInterval , true , 0.0f);
	}
	else
	{
		OnTickPowerUp();
	}
}
