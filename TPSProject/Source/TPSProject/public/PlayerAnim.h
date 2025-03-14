// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float Speed=0 ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = PlayerAnim)
	float Direction = 0 ;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = PlayerAnim)
	bool IsInAir = false ;

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* AttackAnimMontage ;

	void PlayAttackAnim();
};
