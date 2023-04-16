// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LPLioness.h"
#include "LPLionBaby.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPLionBaby : public ALPLioness
{
	GENERATED_BODY()
public:
	ALPLionBaby();
protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
public:
	UFUNCTION()
	void OnPrideOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnRunawayOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnHuntOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnBossStageOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	void OnDestory();

private:
	void OnFatherDeath();
	
};
