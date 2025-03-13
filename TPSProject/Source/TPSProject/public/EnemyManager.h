// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float MinTime = 1 ;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float MaxTime = 5 ;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TArray<class AActor*> SpawnPoints ;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TSubclassOf<class AEnemy> EnemyFactory ;

	FTimerHandle SpawnTimerHandle;

	void CreateEnemy();

	void FindSpawnPoints();

};
