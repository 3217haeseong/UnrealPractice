// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGameModeBase.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	BoxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	MeshComp->SetupAttachment(BoxComp);

	BoxComp->SetCollisionProfileName(TEXT("Bullet"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;

	SetActorLocation(NewLocation);
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyActor* Enemy = Cast<AEnemyActor>(OtherActor);

	if (Enemy != nullptr)
	{
		OtherActor->Destroy();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation(), GetActorRotation());

		AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();

		AShootingGameModeBase* CurrentGameModeBase = Cast<AShootingGameModeBase>(CurrentMode);

		if (CurrentGameModeBase != nullptr)
		{
			CurrentGameModeBase->AddScore(1);
		}
	}

	Destroy();
}



