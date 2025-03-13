// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	void Turn(const struct FInputActionValue& InputValue);
	void LookUp(const struct FInputActionValue& InputValue);

	virtual void SetupInputBinding(class UEnhancedInputComponent* PlayerInput) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	FVector Direction;

	void Move(const struct FInputActionValue& InputValue);

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float WalkSpeed = 200;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float RunSpeed = 600;

	void PlayerMove();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Run;

	void InputRun();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;

	void InputJump(const struct FInputActionValue& InputValue);
};
