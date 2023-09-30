// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "GameFramework/Character.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp -> SetCanEverAffectNavigation(false);
	MeshComp -> SetSimulatePhysics(true);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp ->OnHealthChanged.AddDynamic(this , &ASTrackerBot::HandleTakeDamage);

	bUseVelocityChange = false;
	MovementForce = 1000;

	RequireDistanceToTarget = 100;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPathPoint = GetNextPathPoint();
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp , Log , TEXT("Health %s of %s") , *FString::SanitizeFloat(Health) , *GetName());
}

FVector ASTrackerBot::GetNextPathPoint()
{
	//得到0号玩家
	ACharacter *playerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	//找到通往玩家的路径
	UNavigationPath *NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), playerPawn);

	if(NavPath -> PathPoints.Num() > 1)
	{
		return NavPath -> PathPoints[1];
	}

	return GetActorLocation();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	
	if(DistanceToTarget <= RequireDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= MovementForce;

		MeshComp -> AddForce(ForceDirection , NAME_None , bUseVelocityChange);

		DrawDebugDirectionalArrow(GetWorld() , GetActorLocation() , GetActorLocation() + ForceDirection , 32 , FColor::Yellow , false , 0.0f , 0 , 1.0f );
	}

	DrawDebugSphere(GetWorld() , NextPathPoint , 20 , 12 , FColor::Yellow , false , 0.0f , 1.0f);
}

// Called to bind functionality to input
void ASTrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

