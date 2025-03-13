// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto OwnerPawn = TryGetPawnOwner();
	auto Player = Cast<ATPSPlayer>(OwnerPawn);
	if (Player)
	{
		FVector Velocity = Player->GetVelocity();
		FVector ForwardVector = Player->GetActorForwardVector();
		Speed = FVector::DotProduct(Velocity, ForwardVector);

		FVector RightVector = Player->GetActorRightVector();
		Direction = FVector::DotProduct(Velocity, RightVector);

		auto Movement = Player->GetCharacterMovement();
		IsInAir = Movement->IsFalling();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(AttackAnimMontage);
}
