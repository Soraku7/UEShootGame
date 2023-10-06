// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpActor.h"


// Sets default values
ASPickUpActor::ASPickUpActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCpomp"));
	SphereComp -> SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp -> SetRelativeRotation(FRotator(90 , 0 , 0));
	DecalComp -> DecalSize = FVector(54 , 75 , 75);
	DecalComp -> SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ASPickUpActor::BeginPlay()
{
	Super::BeginPlay();

	ReSpawn();
}

void ASPickUpActor::ReSpawn()
{
	if(PowerUpClass == nullptr)
	{
		UE_LOG(LogTemp , Warning , TEXT("PowerUpClass is nullptr in %s"
								  "Please update your Blueprint") , *GetName());
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	PowerupInstance = GetWorld() -> SpawnActor<ASPowerupActor>(PowerUpClass , GetTransform() , SpawnParams);
}

void ASPickUpActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(PowerupInstance)
	{
		PowerupInstance -> ActivePowerup();

		PowerupInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnHandle , this , &ASPickUpActor::ReSpawn , CooldownDuration);
		
	}
}


