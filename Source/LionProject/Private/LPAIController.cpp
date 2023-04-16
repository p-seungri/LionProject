// Fill out your copyright notice in the Description page of Project Settings.


#include "LPAIController.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>

const FName ALPAIController::HomePosKey(TEXT("HomePos"));
const FName ALPAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ALPAIController::TargetKey(TEXT("Target"));
const FName ALPAIController::IsDeadKey(TEXT("IsDead"));
const FName ALPAIController::AttackKey(TEXT("IsAttack"));
const FName ALPAIController::PatrolKey(TEXT("IsPatrol"));	
const FName ALPAIController::CurrentSkillKey(TEXT("CurrentSkill"));	

ALPAIController::ALPAIController()
{
	bWantsPlayerState = true;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/LPAI/BB_LPAnimal.BB_LPAnimal"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/LPAI/BT_LPAnimal.BT_LPAnimal"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ALPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


}

void ALPAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Destroy();
}

void ALPAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALPAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			LPLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}


void ALPAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

