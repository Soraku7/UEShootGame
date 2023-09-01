// Fill out your copyright notice in the Description page of Project Settings.


#include "Sweapon.h"

#include "IMessageTracer.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASweapon::ASweapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

// Called when the game starts or when spawned
void ASweapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASweapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if(MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		
		//返回pawn眼睛的视角
		MyOwner -> GetActorEyesViewPoint(EyeLocation , EyeRotation);
		FVector ShotDirection = EyeRotation.Vector();
		//准星方向的向量
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		//碰撞忽略 忽略玩家 武器
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		//复合追踪 用于细致追踪到目标网格体的三角形 方便确认位置
		QueryParams.bTraceComplex = true;

		FVector TracerEndPoint = TraceEnd;
		
		//发出射线
		FHitResult Hit;
		if(GetWorld() -> LineTraceSingleByChannel(Hit , EyeLocation , TraceEnd , ECC_Visibility))
		{
			//受到伤害 进行受击处理
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor , 20.0f , ShotDirection , Hit , MyOwner->GetInstigatorController() , this , DamageType);

			if(ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , ImpactEffect , Hit.ImpactPoint , Hit.ImpactNormal.Rotation());

			}

			//射线最终位置
			TracerEndPoint = Hit.ImpactPoint;
		}

		DrawDebugLine(GetWorld() , EyeLocation , TraceEnd , FColor::White , false , 1.0f , 0 , 1.0f);

		if(MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect , MeshComp , MuzzleSocketName);

		}

		if(TracerEffect)
		{
			FVector	MuzzleLocation = MeshComp -> GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , TracerEffect , MuzzleLocation);
			if(TracerComp)
			{
				TracerComp -> SetVectorParameter(TracerTargetName , TracerEndPoint);
			}
		}
	}
	
}

// Called every frame
void ASweapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

