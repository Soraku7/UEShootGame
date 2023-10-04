// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "GameFramework/Character.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/RPCDoSDetection.h"
#include "PhysicsEngine/ShapeElem.h"


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

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp -> SetSphereRadius(200);
	SphereComp -> SetupAttachment(RootComponent);
	SphereComp -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp -> SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp -> SetCollisionResponseToChannel(ECC_Pawn , ECR_Overlap);
	
	bUseVelocityChange = false;
	MovementForce = 1000;

	RequireDistanceToTarget = 100;

	ExplosionRadius = 40;
	ExplosionDamage = 200;

	SelfDamageInterval = 0.25f;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();
	}
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp , Log , TEXT("Health %s of %s") , *FString::SanitizeFloat(Health) , *GetName());

	if(MatIns == nullptr)
	{
		MatIns = MeshComp -> CreateAndSetMaterialInstanceDynamicFromMaterial(0 , MeshComp -> GetMaterial(0));
		
	}
	if(MatIns)
	{
		MatIns -> SetScalarParameterValue("LastTimeDamageTaken" , GetWorld() -> TimeSeconds);
	}

	if(Health <= 0.0f)
	{
		SelfDestruct();
	}
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

void ASTrackerBot::SelfDestruct()
{
	if(bExploded)
	{
		return;
	}

	bExploded = true;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect , GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this , ExplodeSound , GetActorLocation());

	MeshComp -> SetVisibility(false , true);
	MeshComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if(GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this , ExplosionDamage , GetActorLocation()
			, ExplosionRadius , nullptr , IgnoredActors , this,GetInstigatorController() , true);

		DrawDebugSphere(GetWorld() , GetActorLocation() , ExplosionRadius , 12 , FColor::Red , false , 2.0f , 0 , 1.0); 

	
		SetLifeSpan(2.0f);
	}
	
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this , 20 , GetInstigatorController() , this , nullptr);
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetLocalRole() == ROLE_Authority)
	{
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
	
}

// Called to bind functionality to input
void ASTrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(!bStartSelfDestruection && !bExploded)
	{
		ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);

		if(PlayerPawn)
		{
			if(GetLocalRole() == ROLE_Authority && !bExploded)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage , this , &ASTrackerBot::DamageSelf , SelfDamageInterval ,true , 0.0f);

			}
			
			bStartSelfDestruection = true;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}

}

