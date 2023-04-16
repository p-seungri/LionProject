// Fill out your copyright notice in the Description page of Project Settings.


#include "LPLionAnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"

ULPLionAnimInstance::ULPLionAnimInstance()
{
	BiteAttack = false;
	ClawAttack = false;
	CopyAttack = false;
	HowlAttack = false;
	FireAttack = false;
}

void ULPLionAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


}

void ULPLionAnimInstance::SetBiteAttack(bool Bite)
{
	BiteAttack = Bite;
}

void ULPLionAnimInstance::SetClawAttack(bool Claw)
{
	ClawAttack = Claw;
}

void ULPLionAnimInstance::SetCopyAttack(bool Copy)
{
	CopyAttack = Copy;
}

void ULPLionAnimInstance::SetHowlAttack(bool Howl)
{
	HowlAttack = Howl;
}

void ULPLionAnimInstance::SetFireAttack(bool Fire)
{
	FireAttack = Fire;
}


void ULPLionAnimInstance::AnimNotify_BiteCheck()
{
	OnBiteCheck.Broadcast();
}

void ULPLionAnimInstance::AnimNotify_ClawCheck()
{
	OnClawCheck.Broadcast();
}

void ULPLionAnimInstance::AnimNotify_CopyCheck()
{
	OnCopyCheck.Broadcast();
}

void ULPLionAnimInstance::AnimNotify_HowlCheck()
{
	OnHowlCheck.Broadcast();
}

void ULPLionAnimInstance::AnimNotify_FireCheck()
{
	OnFireCheck.Broadcast();
}
