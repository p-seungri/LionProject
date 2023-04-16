// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "AIController.h"
#include "LPAIController.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPAIController : public AAIController
{
	GENERATED_BODY()
public:
	ALPAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void PostInitializeComponents() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName IsDeadKey;
	static const FName AttackKey;
	static const FName PatrolKey;
	static const FName CurrentSkillKey;

	void RunAI();

	void StopAI();

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
