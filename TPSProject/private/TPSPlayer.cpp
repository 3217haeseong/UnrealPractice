// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "EnemyFSM.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),FRotator(0,-90,0));
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 70, 90));
	SpringArmComp->TargetArmLength = 400;
	SpringArmComp->bUsePawnControlRotation = true;

	TpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	TpsCamComp->SetupAttachment(SpringArmComp);
	TpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		GunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	SniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	SniperGunComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));

	if (TempSniperMesh.Succeeded())
	{
		SniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		SniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		SniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	auto PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMC_TPS, 0);
		}
	}

	_SniperUI = CreateWidget(GetWorld(), SniperUIFactory);

	_CrosshairUI = CreateWidget(GetWorld(), CrosshairUIFactory);
	_CrosshairUI->AddToViewport();

	ChangeToSniperGun(FInputActionValue());
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();
	
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput)
	{
		PlayerInput->BindAction(IA_Turn,ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		PlayerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		PlayerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &ATPSPlayer::InputFire);
		PlayerInput->BindAction(IA_GrenadeGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToGrenadeGun);
		PlayerInput->BindAction(IA_SniperGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToSniperGun);
		PlayerInput->BindAction(IA_Sniper, ETriggerEvent::Started, this, &ATPSPlayer::SniperAim);
		PlayerInput->BindAction(IA_Sniper, ETriggerEvent::Completed, this, &ATPSPlayer::SniperAim);
	}

}

void ATPSPlayer::Turn(const FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(const FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerPitchInput(value);
}

void ATPSPlayer::Move(const FInputActionValue& InputValue)
{
	FVector2D Value = InputValue.Get<FVector2D>();

	Direction.X = Value.X;

	Direction.Y = Value.Y;
}

void ATPSPlayer::InputJump(const FInputActionValue& InputValue)
{
	Jump();
}

void ATPSPlayer::PlayerMove()
{
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

void ATPSPlayer::InputFire(const FInputActionValue& InputValue)
{

	if (bUsingGrenadeGun)
	{
		FTransform FirePosition = GunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(BulletFactory, FirePosition);
	}

	else
	{
		FVector StartPos = TpsCamComp->GetComponentLocation();
		FVector EndPos = TpsCamComp->GetComponentLocation() + TpsCamComp->GetForwardVector() * 5000;

		FHitResult HitInfo;

		FCollisionQueryParams params;

		params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, EndPos, ECC_Visibility, params);
		if (bHit)
		{
			FTransform BulletTrans;
			BulletTrans.SetLocation(HitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletEffectFactory, BulletTrans);
			auto HitComp = HitInfo.GetComponent();

			if (HitComp && HitComp->IsSimulatingPhysics())
			{
				FVector dir = (EndPos - StartPos).GetSafeNormal();
				FVector force = dir * HitComp->GetMass() * 500000;
				HitComp->AddForceAtLocation(force, HitInfo.ImpactPoint);
			}

			auto Enemy = HitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (Enemy)
			{
				auto EnemyFSM = Cast<UEnemyFSM>(Enemy);
				EnemyFSM->OnDamageProcess();
			}
		}
	}
}

void ATPSPlayer::ChangeToGrenadeGun(const FInputActionValue& InputValue)
{
	bUsingGrenadeGun = true;
	SniperGunComp->SetVisibility(false);
	GunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun(const FInputActionValue& InputValue)
{
	bUsingGrenadeGun = false;
	SniperGunComp->SetVisibility(true);
	GunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim(const FInputActionValue& InputValue)
{
	if (bUsingGrenadeGun)
	{
		return;
	}

	if (bSniperAim == false)
	{
		bSniperAim = true;

		_SniperUI->AddToViewport();

		TpsCamComp->SetFieldOfView(45.0f);
		_CrosshairUI->RemoveFromParent();
	}

	else
	{
		bSniperAim = false;
		_SniperUI->RemoveFromParent();
		TpsCamComp->SetFieldOfView(90.0f);
		_CrosshairUI->AddToViewport();
	}
}

