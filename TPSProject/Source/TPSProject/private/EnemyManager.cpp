// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	float CreateTime = FMath::RandRange(MinTime, MaxTime);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManager::CreateEnemy, CreateTime);

	FindSpawnPoints();
	
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemy()
{

	int Index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	GetWorld()->SpawnActor<AEnemy>(EnemyFactory, SpawnPoints[Index]->GetActorLocation(), FRotator(0));

	float CreateTime = FMath::RandRange(MinTime, MaxTime);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManager::CreateEnemy, CreateTime);
}

void AEnemyManager::FindSpawnPoints()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

	for (auto Spawn : AllActors)
	{
		if (Spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			SpawnPoints.Add(Spawn);
		}
	}
}

