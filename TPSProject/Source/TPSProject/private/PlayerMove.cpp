// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	MoveComp->MaxWalkSpeed = WalkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerMove();
}


void UPlayerMove::Turn(const FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	Me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(const FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	Me->AddControllerPitchInput(value);
}

void UPlayerMove::SetupInputBinding(UEnhancedInputComponent* PlayerInput)
{
	PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &UPlayerMove::Turn);
	PlayerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &UPlayerMove::LookUp);
	PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UPlayerMove::Move);
	PlayerInput->BindAction(IA_Run, ETriggerEvent::Started, this, &UPlayerMove::InputRun);
	PlayerInput->BindAction(IA_Run, ETriggerEvent::Completed, this, &UPlayerMove::InputRun);
	PlayerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &UPlayerMove::InputJump);
}

void UPlayerMove::Move(const FInputActionValue& InputValue)
{
	FVector2D Value = InputValue.Get<FVector2D>();

	Direction.X = Value.X;

	Direction.Y = Value.Y;
}

void UPlayerMove::PlayerMove()
{
	Direction = FTransform(Me->GetControlRotation()).TransformVector(Direction);
	Me->AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

void UPlayerMove::InputRun()
{
	auto Movement = Me->GetCharacterMovement();

	if (Movement->MaxWalkSpeed > WalkSpeed)
	{
		Movement->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		Movement->MaxWalkSpeed = RunSpeed;
	}
}

void UPlayerMove::InputJump(const FInputActionValue& InputValue)
{
	Me->Jump();
}