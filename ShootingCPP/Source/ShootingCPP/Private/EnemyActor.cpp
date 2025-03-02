// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "ShootingGameModeBase.h"


// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	BoxComp->SetCollisionProfileName(TEXT("Enemy"));
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	int32 DrawResult = FMath::RandRange(1, 100);

	if (DrawResult <= TraceRate)
	{
		for (TActorIterator<APlayerPawn> player(GetWorld()); player; ++player)
		{
			if (player->GetName().Contains(TEXT("BP_PlayerPawn")))
			{
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize();
			}
		}
	}
	else {
		dir = GetActorForwardVector();
	}

	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AEnemyActor::OnEnemyOverlap);
	
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + DeltaTime * dir * MoveSpeed;

	SetActorLocation(NewLocation);
}

void AEnemyActor::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawn* Player = Cast<APlayerPawn>(OtherActor);

	if (Player != nullptr)
	{
		OtherActor->Destroy();

		AShootingGameModeBase* CurrentGameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (CurrentGameMode != nullptr)
		{
			CurrentGameMode->ShowMenu();
		}
	}

	Destroy();
}

