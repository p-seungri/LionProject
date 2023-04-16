// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ULPSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	ULPSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	float Exp;

	
};
