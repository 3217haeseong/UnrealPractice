// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonReStart->OnClicked.AddDynamic(this,&UMenuWidget::Restart);
	ButtonQuit->OnClicked.AddDynamic(this, &UMenuWidget::Quit);
}

void UMenuWidget::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(), "ShootingMap");
}

void UMenuWidget::Quit()
{
	UWorld* CurrentWorld = GetWorld();

	UKismetSystemLibrary::QuitGame(CurrentWorld, CurrentWorld->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
