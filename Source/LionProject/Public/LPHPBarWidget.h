// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "Blueprint/UserWidget.h"
#include "LPHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ULPHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindPlayerState(class ALPPlayerState* NewPlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateLevelWidget();

private:
	TWeakObjectPtr<class ALPPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPProgressBar;

	UPROPERTY()
	class UTextBlock* LevelTextBox;
};
