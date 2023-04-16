// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "LPAIController.h"
#include <LPAnimal.h>
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ALPAnimal>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Enemy)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<ALPAnimal>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALPAIController::TargetKey));

	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - Enemy->GetActorLocation();
	LookVector.Z = 0;

	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
