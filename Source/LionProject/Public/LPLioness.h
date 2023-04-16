// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "LPLion.h"
#include "LPLioness.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPLioness : public ALPLion
{
	GENERATED_BODY()
public:
	ALPLioness();
protected:
	virtual void BeginPlay() override;
public:


};
