// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonReStart ;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ButtonQuit ;

	UFUNCTION()
	void Restart() ;

	UFUNCTION()
	void Quit() ;
};
