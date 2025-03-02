// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"

void AShootingGameModeBase::AddScore(int32 point)
{
	CurrentScore += point;

	PrintScore();
}

void AShootingGameModeBase::ShowMenu()
{
	if (MenuWidget != nullptr)
	{
		MenuUI = CreateWidget<UMenuWidget>(GetWorld(), MenuWidget);

		if (MenuUI)
		{
			MenuUI->AddToViewport();

			UGameplayStatics::SetGamePaused(GetWorld(), true);

			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
}

void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidget != nullptr)
	{
		MainUI = CreateWidget<UMainWidget>(GetWorld(), MainWidget);

		if (MainUI != nullptr)
		{
			MainUI->AddToViewport();
		}
	}
}

void AShootingGameModeBase::PrintScore()
{
	if (MainUI != nullptr)
	{
		MainUI->ScoreData->SetText(FText::AsNumber(CurrentScore));
	}
}
