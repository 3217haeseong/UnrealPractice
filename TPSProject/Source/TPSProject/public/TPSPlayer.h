// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*);

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

	UPROPERTY(VisibleAnywhere,Category=GunMesh)
	class USkeletalMeshComponent* GunMeshComp ;

	UPROPERTY(VisibleAnywhere,Category = GunMesh)
	class UStaticMeshComponent* SniperGunComp ;


public:
	UPROPERTY(VisibleAnywhere,Category = Component)
	class UPlayerBaseComponent* playerMove ;

	/*UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerFire ;*/

public:
	FInputBindingDelegate OnInputBindingDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= Health)
	int32 hp ;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Health)
	int32 initialHp = 10 ;

	UFUNCTION(BlueprintCallable,Category= Health)
	void OnHitEvent() ;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = Health)
	void OnGameOver() ;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category = Health)
	void OnUsingGrenade(bool IsGrenade) ;
};
