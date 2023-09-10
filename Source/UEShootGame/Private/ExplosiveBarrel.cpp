// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp -> OnHealthChanged.AddDynamic(this , &AExplosiveBarrel::OnHealthChanged);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//启用物理模拟
	MeshComp -> SetSimulatePhysics(true);
	MeshComp -> SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp -> SetupAttachment(MeshComp);
	RadialForceComp -> Radius = 250;
	RadialForceComp -> bImpulseVelChange = true;
	RadialForceComp -> bAutoActivate = false;
	RadialForceComp -> bIgnoreOwningActor = true;

	ExplosionImpulse = 400;
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExplosiveBarrel::OnHealthChanged(USHealthComponent* HealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(bExploded)
	{
		return;
	}

	if(Health <= 0.0f)
	{
		//爆炸
		bExploded = true;

		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		//添加冲击力
		MeshComp -> AddImpulse(BoostIntensity , NAME_None , true);

		//播放VFX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , ExplosionEffect , GetActorLocation());

		MeshComp -> SetMaterial(0 , ExplodedMaterial);

		RadialForceComp -> FireImpulse();
	}
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

