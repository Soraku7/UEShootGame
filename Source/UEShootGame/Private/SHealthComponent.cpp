﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	DefaultHealth = 100;

	SetIsReplicated(true);
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	 if(GetOwnerRole() == ROLE_Authority)
	 {
	 	AActor* MyOnwer = GetOwner();
	 	if(MyOnwer)
	 	{
	 		//添加委托
	 		MyOnwer -> OnTakeAnyDamage.AddDynamic(this , &USHealthComponent::HandleTakeAnyManage);
	 	}
	 }
	
	Health = DefaultHealth;
}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this , Health , Damage, nullptr , nullptr , nullptr);
}

void USHealthComponent::HandleTakeAnyManage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                            AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f)
	{
		return;
	}

	//更新生命值   将血量限制在0到默认血量之间
	Health = FMath::Clamp(Health - Damage , 0 , DefaultHealth);

	UE_LOG(LogTemp , Log , TEXT("Health Changed: %s") , *FString::SanitizeFloat(Health));
	
	OnHealthChanged.Broadcast(this , Health , Damage , DamageType , InstigatedBy , DamageCauser);
}

void USHealthComponent::Heal(float HealAmount)
{
	if(HealAmount <= 0 && Health <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount , 0.0f , DefaultHealth);
	UE_LOG(LogTemp , Log , TEXT("Health Changed: %s(+%s)") , *FString::SanitizeFloat(Health) , *FString::SanitizeFloat(HealAmount));

	OnHealthChanged.Broadcast(this , Health , HealAmount , nullptr , nullptr , nullptr);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}
