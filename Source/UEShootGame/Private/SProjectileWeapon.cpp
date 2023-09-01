// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileWeapon.h"


// Sets default values
ASProjectileWeapon::ASProjectileWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASProjectileWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if(MyOwner && ProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner -> GetActorEyesViewPoint(EyeLocation , EyeRotation);
		
		FVector MuzzleLocation = MeshComp -> GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = MeshComp -> GetSocketRotation(MuzzleSocketName);

		//设置碰撞
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//创建Actor
		GetWorld() -> SpawnActor<AActor>(ProjectileClass , MuzzleLocation , EyeRotation , SpawnParameters);
	}
}

// Called every frame
void ASProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

