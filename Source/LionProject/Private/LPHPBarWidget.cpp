// Fill out your copyright notice in the Description page of Project Settings.


#include "LPHPBarWidget.h"
#include <LPPlayerState.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void ULPHPBarWidget::BindPlayerState(ALPPlayerState* NewPlayerState)
{
	CurrentPlayerState = NewPlayerState;

	NewPlayerState->OnHPIsChanged.AddUObject(this, &ULPHPBarWidget::UpdateHPWidget);
	NewPlayerState->OnLevelIsChanged.AddUObject(this, &ULPHPBarWidget::UpdateLevelWidget);
	NewPlayerState->OnLevelIsChanged.AddUObject(this, &ULPHPBarWidget::UpdateHPWidget);
}

void ULPHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LevelTextBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Level")));
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));

	if (HPProgressBar != nullptr)
	{
		UpdateHPWidget();
	}

	if (LevelTextBox != nullptr)
	{
		UpdateLevelWidget();
	}
}

void ULPHPBarWidget::UpdateHPWidget()
{
	if (CurrentPlayerState.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentPlayerState->GetHPRatio());
		}
	}
}

void ULPHPBarWidget::UpdateLevelWidget()
{
	if (CurrentPlayerState.IsValid())
	{
		if (nullptr != LevelTextBox)
		{
			LevelTextBox->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
		}
	}
}
