// Fill out your copyright notice in the Description page of Project Settings.


#include "LPGameModeBase.h"
#include "LPPlayerController.h"
#include "LPLionBaby.h"
#include <LPPlayerState.h>
#include <LPGameState.h>


ALPGameModeBase::ALPGameModeBase()
{
	PlayerControllerClass = ALPPlayerController::StaticClass();
	DefaultPawnClass = ALPLionBaby::StaticClass();
	PlayerStateClass = ALPPlayerState::StaticClass();
	GameStateClass = ALPGameState::StaticClass();

}


void ALPGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto LPPlayerState = Cast<ALPPlayerState>(NewPlayer->PlayerState);

	if (nullptr != LPPlayerState)
	{
		LPPlayerState->InitPlayerData();
	}
}

void ALPGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
