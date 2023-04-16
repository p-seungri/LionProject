// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "LPAnimal.h"
#include "LPElephant.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPElephant : public ALPAnimal
{
	GENERATED_BODY()
public:
	ALPElephant();
protected:
	virtual void PostInitializeComponents() override;
public:
	UPROPERTY(VisibleAnywhere, Category = Pawn)
	USkeletalMeshComponent* SK_ElephantTusksSmall;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	USkeletalMeshComponent* SK_ElephantTusksMedium;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	USkeletalMeshComponent* SK_ElephantTusksBig;
};
