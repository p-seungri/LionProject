// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "LPAnimal.h"
#include "LPRhino.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPRhino : public ALPAnimal
{
	GENERATED_BODY()
public:
	ALPRhino();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
private:
	float CurrentSpeed;
	FVector MovementDirection;
public:
	UFUNCTION()
	void OnPlayerOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnDestroy(AActor* DestroyedActor);
};
