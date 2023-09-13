// Fill out your copyright notice in the Description page of Project Settings.


#include "Sweapon.h"
#include "UEShootGame/UEShootGame.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
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

	BaseDamage = 20.0f;
	RateOfFire = 600;
 
	//允许当前Actor可以被服务器复制
	SetReplicates(true);

	//帧数同步
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void ASweapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}


void ASweapon::OnRep_HitScanTrace()
{
	PlayFireEffect(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType , HitScanTrace.TraceTo);
}

void ASweapon::Fire()
{
	//如果不是在服务器上 就执行ServerFire
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}
	
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
		QueryParams.bReturnPhysicalMaterial = true;
		
		FVector TracerEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;
		//发出射线
		FHitResult Hit;
		if(GetWorld() -> LineTraceSingleByChannel(Hit , EyeLocation , TraceEnd , COLLISION_WEAPON , QueryParams))
		{
			//获取受击Actor
			AActor* HitActor = Hit.GetActor();
			
			//获取SurfaceType
			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			
			float ActualDamage = BaseDamage;
			if(SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 4.0f;
			}
			

			//造成伤害
			UGameplayStatics::ApplyPointDamage(HitActor , ActualDamage , ShotDirection , Hit , MyOwner->GetInstigatorController() , this , DamageType);

			PlayImpactEffects(SurfaceType , Hit.Location);
			
			//射线最终位置
			TracerEndPoint = Hit.ImpactPoint;
		}

		DrawDebugLine(GetWorld() , EyeLocation , TraceEnd , FColor::White , false , 1.0f , 0 , 1.0f);

		PlayFireEffect(TracerEndPoint);

		if(GetLocalRole() == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;

		}
	}

	//确定射击时间
	LastFireTime = GetWorld() -> TimeSeconds;
}

void ASweapon::ServerFire_Implementation()
{
	Fire();
}

bool ASweapon::ServerFire_Validate()
{
	return true;
}

void ASweapon::StartFire()
{
	float FirstDelay = FMath::Max(0.0f ,  LastFireTime + TimeBetweenShots - GetWorld() -> TimeSeconds);
	//FirstDelay 如果为负 则在后续函数为0
	//设置FirestDelay 防止连点快速射击
	//设置定时器
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots ,this , &ASweapon::Fire , TimeBetweenShots , true , FirstDelay);
}

void ASweapon::StopFire()
{
	//暂停定时器
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ASweapon::PlayFireEffect(FVector TraceEnd)
{
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
			//修改粒子参数
			TracerComp -> SetVectorParameter(TracerTargetName , TraceEnd);
		}
	}

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if(MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner -> GetController());
		if(PC)
		{
			PC -> ClientStartCameraShake(FireCamShake);
		}
	}
}

void ASweapon::PlayImpactEffects(EPhysicalSurface SurfaceType , FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}
	if(SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp -> GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , SelectedEffect , ImpactPoint , ShotDirection.Rotation());

	}
}

void ASweapon::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//同步本类中CurrentWeapon
	DOREPLIFETIME_CONDITION( ASweapon, HitScanTrace , COND_SkipOwner);
}

