// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsAttackable.h"
#include <LPAIController.h>
#include <LPAnimal.h>
#include <LPAnimalAnimInstance.h>
#include <LPPlayerState.h>
#include <LPLion.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTDecorator_IsAttackable::UBTDecorator_IsAttackable()
{
	NodeName = TEXT("IsAttackable");
}

bool UBTDecorator_IsAttackable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ALPAnimal>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == ControllingPawn)
	{
		
		return false;
	}

	auto Target = Cast<ALPAnimal>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALPAIController::TargetKey));

	if (nullptr == Target)
	{
		return false;
	}

	int32 IndexMin = 0;
	int32 IndexMax = 0;

	auto LPPlayerState = ControllingPawn->GetLPPlayerState();

	if (LPPlayerState != nullptr)
	{
		switch (ControllingPawn->GetAnimalType())
		{
		case EAnimalType::Lion:

			switch (LPPlayerState->GetCharacterLevel())
			{
			case 9:
				IndexMin = 0u;
				IndexMax = 4u;
				break;
			case 8:
			case 7:
				IndexMin = 0u;
				IndexMax = 3u;
				break;
			case 6:
			case 5:
				IndexMin = 0u;
				IndexMax = 2u;
				break;
			case 4:
			case 3:
				IndexMin = 0u;
				IndexMax = 1u;
				break;
			case 2:
			case 1:
				IndexMin = 0u;
				IndexMax = 0u;
				break;
			default:
				IndexMin = 0u;
				IndexMax = 4u;
				break;
			}
			break;
		default:
			IndexMin = 7u;
			IndexMax = 7u;
			break;
		}



		ELPSkills SkillType = static_cast<ELPSkills>(FMath::RandRange(IndexMin, IndexMax));

		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ALPAIController::CurrentSkillKey, (int32)SkillType);

		float CurrentRange = 100.0f;



		switch (SkillType)
		{
		case ELPSkills::LionBite:
		{
			auto LPLion = Cast<ALPLion>(ControllingPawn);

			CurrentRange = LPLion->GetBiteRange();
		}
		break;
		case ELPSkills::LionClaw:
		{
			auto LPLion = Cast<ALPLion>(ControllingPawn);

			CurrentRange = LPLion->GetClawRange();
		}
		break;
		case ELPSkills::LionCopy:
		{
			auto LPLion = Cast<ALPLion>(ControllingPawn);

			CurrentRange = LPLion->GetCopyRange();
		}
		break;
		case ELPSkills::LionHowl:
		{
			auto LPLion = Cast<ALPLion>(ControllingPawn);

			CurrentRange = LPLion->GetHowlRange();
		}
		break;
		case ELPSkills::LionFire:
		{
			auto LPLion = Cast<ALPLion>(ControllingPawn);

			CurrentRange = LPLion->GetFireRange();
		}
		break;
		case ELPSkills::Others:
			CurrentRange = 200.0f * LPPlayerState->GetScale();
			break;

		}


		bResult = (Target->GetDistanceTo(ControllingPawn) <= CurrentRange + 50.0f);
	}


	return bResult;
}
