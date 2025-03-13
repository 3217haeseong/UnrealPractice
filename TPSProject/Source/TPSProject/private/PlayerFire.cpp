// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "EnemyFSM.h"
#include <Camera/CameraComponent.h>
#include "PlayerAnim.h"

UPlayerFire::UPlayerFire()
{
	ConstructorHelpers::FObjectFinder<USoundBase> TempSound(TEXT("SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (TempSound.Succeeded())
	{
		BulletSound = TempSound.Object;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	_SniperUI = CreateWidget(GetWorld(), SniperUIFactory);

	TpsCamComp = Me->TpsCamComp;
	GunMeshComp = Me->GunMeshComp;
	SniperGunComp = Me->SniperGunComp;

	_CrosshairUI = CreateWidget(GetWorld(), CrosshairUIFactory);
	_CrosshairUI->AddToViewport();

	ChangeToSniperGun(FInputActionValue());
}

void UPlayerFire::SetupInputBinding(UEnhancedInputComponent* PlayerInput)
{
	PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &UPlayerFire::InputFire);
	PlayerInput->BindAction(IA_GrenadeGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInput->BindAction(IA_SniperGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToSniperGun);
	PlayerInput->BindAction(IA_Sniper, ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
	PlayerInput->BindAction(IA_Sniper, ETriggerEvent::Completed, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::ChangeToGrenadeGun(const FInputActionValue& InputValue)
{
	bUsingGrenadeGun = true;
	SniperGunComp->SetVisibility(false);
	GunMeshComp->SetVisibility(true);
	Me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::ChangeToSniperGun(const FInputActionValue& InputValue)
{
	bUsingGrenadeGun = false;
	SniperGunComp->SetVisibility(true);
	GunMeshComp->SetVisibility(false);
	Me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::SniperAim(const FInputActionValue& InputValue)
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

void UPlayerFire::InputFire(const FInputActionValue& InputValue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);

	auto PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);

	auto Anim = Cast <UPlayerAnim>(Me->GetMesh()->GetAnimInstance());
	Anim->PlayAttackAnim();


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

		params.AddIgnoredActor(Me);

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