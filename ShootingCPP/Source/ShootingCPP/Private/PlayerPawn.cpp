// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));

	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

	MeshComp->SetupAttachment(BoxComp);

	FVector BoxSize = FVector(50.0f, 50.0f, 50.0f);
	BoxComp->SetBoxExtent(BoxSize);

	FirePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));
	FirePosition->SetupAttachment(BoxComp);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	BoxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		if (SubSys != nullptr)
		{
			SubSys->AddMappingContext(IMC_PlayerInput, 0);
		}
	}
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dir = FVector(0, h, v);
	dir.Normalize();

	FVector NewLocation = GetActorLocation() + dir * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation, true);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(IA_Horizontal, ETriggerEvent::Triggered, this, &APlayerPawn::OnInputHorizontal);
		EnhancedInputComponent->BindAction(IA_Horizontal, ETriggerEvent::Completed, this, &APlayerPawn::OnInputHorizontal);
		EnhancedInputComponent->BindAction(IA_Vertical, ETriggerEvent::Triggered, this, &APlayerPawn::OnInputVertical);
		EnhancedInputComponent->BindAction(IA_Vertical, ETriggerEvent::Completed, this, &APlayerPawn::OnInputVertical);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &APlayerPawn::Fire);
	}

}

void APlayerPawn::OnInputHorizontal(const FInputActionValue& value)
{
	//float hor = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT(" Horizontal: %.2f"), hor);

	h = value.Get<float>();
}

void APlayerPawn::OnInputVertical(const FInputActionValue& value)
{
	//float ver = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT(" Vertical: %.2f"), ver);

	v = value.Get<float>();
}

void APlayerPawn::Fire()
{
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletFactory, FirePosition->GetComponentLocation(), FirePosition->GetComponentRotation());

	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
}

