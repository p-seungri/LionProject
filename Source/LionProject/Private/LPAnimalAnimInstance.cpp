// Fill out your copyright notice in the Description page of Project Settings.


#include "LPAnimalAnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"

ULPAnimalAnimInstance::ULPAnimalAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	IsSprint = false;
	IsHit = false;
	IsAttacking = false;
	IsRest = false;
}

void ULPAnimalAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	auto Pawn = TryGetPawnOwner();

	if (!IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		IsInAir = Pawn->GetMovementComponent()->IsFalling();

		//auto TopStateMachine = GetStateMachineInstanceFromName(TEXT("Top State Machine"));

		//if (TopStateMachine != nullptr)
		//{
		//	if (TopStateMachine->GetCurrentStateName() == TEXT("Hit"))
		//	{
		//		auto MachineIndex = GetStateMachineIndex(TEXT("Top State Machine"));
		//		auto StateIndex = TopStateMachine->GetCurrentState();

		//		auto RTF = GetRelevantAnimTimeRemainingFraction(MachineIndex, StateIndex);

		//		if (RTF < 0.1)
		//		{
		//			IsHit = false;
		//		}

		//	}
		//}
	}
}

void ULPAnimalAnimInstance::SetIsSprint(bool Sprint)
{
	IsSprint = Sprint;
}

void ULPAnimalAnimInstance::SetIsHit(bool Hit)
{
	IsHit = Hit;
}

void ULPAnimalAnimInstance::SetIsAttacking(bool Attacking)
{
	IsAttacking = Attacking;
}

void ULPAnimalAnimInstance::SetIsRest(bool Rest)
{
	IsRest = Rest;

	if (IsRest)
	{
		OnRestStart.Broadcast();
	}
	else
	{
		OnRestEnd.Broadcast();
	}
}

bool ULPAnimalAnimInstance::GetIsRest() const
{
	return IsRest;
}

bool ULPAnimalAnimInstance::GetIsDead() const
{
	return IsDead;
}

bool ULPAnimalAnimInstance::GetIsAttacking() const
{
	return IsAttacking;
}

void ULPAnimalAnimInstance::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}

void ULPAnimalAnimInstance::AnimNotify_AttackCheck()
{
	OnAttackCheck.Broadcast();
}

void ULPAnimalAnimInstance::AnimNotify_HitEnd()
{
	OnHitEnd.Broadcast();
}
