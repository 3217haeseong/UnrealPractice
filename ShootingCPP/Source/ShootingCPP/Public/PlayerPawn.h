// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SHOOTINGCPP_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp ;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp ;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 500 ;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* FirePosition ;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletFactory ;

	UPROPERTY(EditAnywhere)
	class USoundBase* FireSound ;
	
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* IMC_PlayerInput ;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Horizontal ;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Vertical ;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Fire;

private:

	float h;
	float v;

	void OnInputHorizontal(const struct FInputActionValue& value);
	void OnInputVertical(const struct FInputActionValue& value);

	void Fire();
};
