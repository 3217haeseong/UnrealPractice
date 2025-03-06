// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	CollisionComp->SetSphereRadius(13);

	RootComponent = CollisionComp;

	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	BodyMeshComp->SetupAttachment(RootComponent);
	BodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(CollisionComp);
	MovementComp->InitialSpeed = 5000;
	MovementComp->MaxSpeed = 5000;
	MovementComp->bShouldBounce = true;
	MovementComp->Bounciness = 0.3f;

	//InitialLifeSpan = 2.0f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DeathTimer;
	//GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ABullet::Die, 2.0f, false);

	GetWorld()->GetTimerManager().SetTimer(DeathTimer, FTimerDelegate::CreateLambda([this]()->void {
		Destroy();
		}), 2.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	Destroy();
}

