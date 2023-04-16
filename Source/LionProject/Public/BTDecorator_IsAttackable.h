// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAttackable.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API UBTDecorator_IsAttackable : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsAttackable();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
