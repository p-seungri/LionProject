// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "GameFramework/GameStateBase.h"
#include "LPGameState.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ALPGameState();

	void SetGameCleared();
	bool IsGameCleared() const;
	void SetStageCleared();
	bool IsStageCleared() const;

private:


	UPROPERTY(Transient)
	bool bGameCleared;

	UPROPERTY(Transient)
	bool bStageCleared;
};
