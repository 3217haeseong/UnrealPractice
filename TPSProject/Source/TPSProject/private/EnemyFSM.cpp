// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include "Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),ATPSPlayer::StaticClass());
	Target = Cast<ATPSPlayer>(actor);
	Me = Cast<AEnemy>(GetOwner());
	
	Anim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());

	AI = Cast<AAIController>(Me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString LogMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, LogMsg);

	switch (mState)
	{
	case EEnemyState::Idle :
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleDelayTime)
	{
		mState = EEnemyState::Move;
		CurrentTime = 0;
	}

	Anim->AnimState = mState;
	GetRandomPositionInNavMesh(Me->GetActorLocation(), 500, RandomPos);
}

void UEnemyFSM::MoveState()
{
	FVector Destination = Target->GetActorLocation();
	FVector Dir = Destination - Me->GetActorLocation();

	auto NS = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery Query;
	FAIMoveRequest Req;

	Req.SetAcceptanceRadius(3);
	Req.SetGoalLocation(Destination);
	AI->BuildPathfindingQuery(Req, Query);
	FPathFindingResult r = NS->FindPathSync(Query);

	if (r.Result == ENavigationQueryResult::Success)
	{
		AI->MoveToLocation(Destination);
	}
	else
	{
		auto Result = AI->MoveToLocation(RandomPos);
		if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(Me->GetActorLocation(), 500, RandomPos);
		}
	}

	if (Dir.Size() < AttackRange)
	{
		AI->StopMovement();

		mState = EEnemyState::Attack;
		Anim->AnimState = mState;
		Anim->bAttackPlay = true;
		CurrentTime = AttackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > AttackDelayTime)
	{
		//PRINT_LOG(TEXT("Attack!!!!!"));
		CurrentTime = 0;
		Anim->bAttackPlay = true;
	}

	float Distance = FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation());

	if (Distance > AttackRange)
	{
		mState = EEnemyState::Move;
		Anim->AnimState = mState;

		GetRandomPositionInNavMesh(Me->GetActorLocation(), 500, RandomPos);
	}
}

void UEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if(CurrentTime > DamageDelayTime)
	{
		mState = EEnemyState::Idle;
		CurrentTime = 0;
		Anim->AnimState = mState;
	}
}

void UEnemyFSM::DieState()
{
	if (Anim->bDieDone == false)
	{
		return;
	}
	FVector P0 = Me->GetActorLocation();
	FVector vt = FVector::DownVector * DieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;

	Me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		Me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		int32 Index = FMath:: RandRange(0, 1);
		FString SectionName = FString::Printf(TEXT("Damage%d"), Index);
		Anim->PlayDamageAnim(FName(*SectionName));
	}

	else
	{
		mState = EEnemyState::Die;
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Anim->PlayDamageAnim(TEXT("Die"));
	}
	Anim->AnimState = mState;
	AI->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& Dest)
{
	auto NS = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool Result = NS->GetRandomReachablePointInRadius(CenterLocation, Radius, loc);
	Dest = loc.Location;

	return false;
}

