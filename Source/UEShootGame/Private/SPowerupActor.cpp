// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

#include "Net/UnrealNetwork.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNrOfTicks = 0;

	bIsPowerupActive = false;
	
	SetReplicates(true);
}

void ASPowerupActor::OnTickPowerUp()
{
	TickProcessed ++;

	OnPowerupTicked();
	
	if(TickProcessed > TotalNrOfTicks)
	{
		OnExpire();

		bIsPowerupActive = false;
		OnRep_PowerupActive();
		
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerupActor::ActivatePowerup(AActor* ActiveFor)
{
	OnActivated(ActiveFor);

	bIsPowerupActive = true;
	OnRep_PowerupActive();
	
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

void ASPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChange(bIsPowerupActive);
}

void ASPowerupActor::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//同步本类中CurrentWeapon
	DOREPLIFETIME(ASPowerupActor , bIsPowerupActive);
}
