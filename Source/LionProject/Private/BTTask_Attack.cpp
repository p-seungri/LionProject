// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include <LPAIController.h>
#include <LPAnimal.h>
#include <LPPlayerState.h>
#include <LPAnimalAnimInstance.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	NodeName = TEXT("Attack");
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ALPAnimal>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Enemy)
	{
		return EBTNodeResult::Failed;
	}

	if (!IsAttacking)
	{
		IsAttacking = true;
		Enemy->Attack(static_cast<ELPSkills>(OwnerComp.GetBlackboardComponent()->GetValueAsInt(ALPAIController::CurrentSkillKey)));

		FTimerHandle DelayHandle;

		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				IsAttacking = false;
			}), 0.1f, false);

		/*Enemy->AnimalAnim->OnAttackEnd.AddLambda([this]() -> void
			{
				IsAttacking = false;
			});*/

	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}
