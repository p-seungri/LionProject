// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "LPAnimal.h"
#include "LPCrocodile.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPCrocodile : public ALPAnimal
{
	GENERATED_BODY()
public:
	ALPCrocodile();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:



};
