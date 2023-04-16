// Fill out your copyright notice in the Description page of Project Settings.


#include "LPGameState.h"


ALPGameState::ALPGameState()
{
	bGameCleared = false;
	bStageCleared = false;
}


void ALPGameState::SetGameCleared()
{
	bGameCleared = true;
}

bool ALPGameState::IsGameCleared() const
{
	return bGameCleared;
}

void ALPGameState::SetStageCleared()
{
	bStageCleared = true;
}

bool ALPGameState::IsStageCleared() const
{
	return bStageCleared;
}
