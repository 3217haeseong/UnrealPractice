// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent), meta=(BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* PlayerInput) override;

	UPROPERTY()
	class UCameraComponent* TpsCamComp;

	UPROPERTY()
	class USkeletalMeshComponent* GunMeshComp;

	UPROPERTY()
	class UStaticMeshComponent* SniperGunComp;

	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> BulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	void InputFire(const struct FInputActionValue& InputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_GrenadeGun;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SniperGun;

	bool bUsingGrenadeGun = true;
	void ChangeToGrenadeGun(const struct FInputActionValue& InputValue);
	void ChangeToSniperGun(const struct FInputActionValue& InputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sniper;

	void SniperAim(const struct FInputActionValue& InputValue);
	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY()
	class UUserWidget* _SniperUI;

	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* BulletEffectFactory;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> CrosshairUIFactory;

	UPROPERTY()
	class UUserWidget* _CrosshairUI;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* BulletSound;

	
};
