// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "GameFramework/GameMode.h"
#include "LPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALPGameModeBase();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void PostInitializeComponents() override;
private:
};
