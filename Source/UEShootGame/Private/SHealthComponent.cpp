// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	DefaultHealth = 100;
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* MyOnwer = GetOwner();
	if(MyOnwer)
	{
		//添加委托
		MyOnwer -> OnTakeAnyDamage.AddDynamic(this , &USHealthComponent::HandleTakeAnyManage);
	}

	Health = DefaultHealth;
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

