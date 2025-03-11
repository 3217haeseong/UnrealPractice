// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere,Category=Camera)
	class USpringArmComponent* SpringArmComp ;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera)
	class UCameraComponent* TpsCamComp ;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	class UInputMappingContext* IMC_TPS ;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp ;

	UPROPERTY(EditDefaultsOnly,Category= "Input")
	class UInputAction* IA_Turn ;

	void Turn(const struct FInputActionValue& InputValue);
	void LookUp(const struct FInputActionValue& InputValue);

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	class UInputAction* IA_Move ;

	UPROPERTY(EditAnywhere,Category = PlayerSetting)
	float WalkSpeed = 600 ;

	FVector Direction;

	void Move(const struct FInputActionValue& InputValue);

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	class UInputAction* IA_Jump ;

	void InputJump(const struct FInputActionValue& InputValue);

	void PlayerMove();

	UPROPERTY(VisibleAnywhere,Category=GunMesh)
	class USkeletalMeshComponent* GunMeshComp ;

	UPROPERTY(EditDefaultsOnly,Category = BulletFactory)
	TSubclassOf<class ABullet> BulletFactory ;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	class UInputAction* IA_Fire ;

	void InputFire(const struct FInputActionValue& InputValue);

	UPROPERTY(VisibleAnywhere,Category = GunMesh)
	class UStaticMeshComponent* SniperGunComp ;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	class UInputAction* IA_GrenadeGun ;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SniperGun ;

	bool bUsingGrenadeGun = false;
	void ChangeToGrenadeGun(const struct FInputActionValue& InputValue);
	void ChangeToSniperGun(const struct FInputActionValue& InputValue);

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	class UInputAction* IA_Sniper ;

	void SniperAim(const struct FInputActionValue& InputValue);
	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly,Category= SniperUI)
	TSubclassOf<class UUserWidget> SniperUIFactory ;

	UPROPERTY()
	class UUserWidget* _SniperUI;

	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* BulletEffectFactory ;

	UPROPERTY(EditDefaultsOnly,Category = SniperUI)
	TSubclassOf<class UUserWidget> CrosshairUIFactory ;

	UPROPERTY()
	class UUserWidget* _CrosshairUI ;
};
