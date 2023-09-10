﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UEShootGame/Public/SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UEShootGame/UEShootGame.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	CameraComp -> SetupAttachment(SpringArmComp);
	
	SpringArmComp -> SetupAttachment(RootComponent);
	//摄像机跟随玩家视角转动
	SpringArmComp -> bUsePawnControlRotation = true;

	GetMovementComponent() -> GetNavAgentPropertiesRef().bCanCrouch = true;
	
	GetCapsuleComponent() -> SetCollisionResponseToChannel(COLLISION_WEAPON , ECR_Ignore);
	ZoomedFOV = 65.0f;
	ZoomInterSpeed = 20.0f;
	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	//获取默认视场
	DefaultFOV = CameraComp -> FieldOfView;

	//设置碰撞参数
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//设置默认武器
	CurrentWeapon = GetWorld() -> SpawnActor<ASweapon>(StartWeaponClass , FVector::ZeroVector , FRotator::ZeroRotator , SpawnParameters);

	if(CurrentWeapon)
	{
		CurrentWeapon -> SetOwner(this);
		CurrentWeapon -> AttachToComponent(GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , "WeaponSocket");
	}

	HealthComp -> OnHealthChanged.AddDynamic(this , &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value); 
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
	if(CurrentWeapon)
	{
		CurrentWeapon -> StartFire();
	}
}

void ASCharacter::StopFire()
{
	if(CurrentWeapon)
	{
		CurrentWeapon -> StopFire();
	}
}

void ASCharacter::OnHealthChanged(USHealthComponent* HealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(Health <= 0.0f && !bDied)
	{
		bDied = true;
		
		//停止目标移动
		GetMovementComponent() -> StopMovementImmediately();
		GetCapsuleComponent() -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//销毁Pawn
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp -> FieldOfView , TargetFOV , DeltaTime , ZoomInterSpeed);
	
	CameraComp -> SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent -> BindAxis("MoveForward" , this , &ASCharacter::MoveForward);
	PlayerInputComponent -> BindAxis("MoveRight" , this , &ASCharacter::MoveRight);
	
	PlayerInputComponent -> BindAxis("LookUp" , this , &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent -> BindAxis("Turn" , this , &ASCharacter::AddControllerYawInput);

	PlayerInputComponent -> BindAction("Crouch" , IE_Pressed , this , &ASCharacter::BeginCrouch);
	PlayerInputComponent -> BindAction("Crouch" , IE_Released , this , &ASCharacter::EndCrouch);

	PlayerInputComponent -> BindAction("Jump" , IE_Pressed , this , &ASCharacter::Jump);

	PlayerInputComponent -> BindAction("Zoom" , IE_Pressed , this , &ASCharacter::BeginZoom);
	PlayerInputComponent -> BindAction("Zoom" , IE_Released , this , &ASCharacter::EndZoom);

	PlayerInputComponent -> BindAction("Fire" , IE_Pressed , this , &ASCharacter::StartFire);
	PlayerInputComponent -> BindAction("Fire" , IE_Released , this , &ASCharacter::StopFire);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if(CameraComp)
	{
		return CameraComp -> GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

