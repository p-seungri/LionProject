// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "LPAnimalAnimInstance.h"
#include "LPLionAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnBiteCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnClawCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCopyCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHowlCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnFireCheckDelegate);

UCLASS()
class LIONPROJECT_API ULPLionAnimInstance : public ULPAnimalAnimInstance
{
	GENERATED_BODY()
public:
	ULPLionAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetBiteAttack(bool Bite);
	void SetClawAttack(bool Claw);
	void SetCopyAttack(bool Copy);
	void SetHowlAttack(bool Howl);
	void SetFireAttack(bool Fire);

	FOnBiteCheckDelegate OnBiteCheck;
	FOnClawCheckDelegate OnClawCheck;
	FOnCopyCheckDelegate OnCopyCheck;
	FOnHowlCheckDelegate OnHowlCheck;
	FOnFireCheckDelegate OnFireCheck;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool BiteAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool ClawAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool CopyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool HowlAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool FireAttack;

	UFUNCTION()
	void AnimNotify_BiteCheck();

	UFUNCTION()
	void AnimNotify_ClawCheck();

	UFUNCTION()
	void AnimNotify_CopyCheck();

	UFUNCTION()
	void AnimNotify_HowlCheck();

	UFUNCTION()
	void AnimNotify_FireCheck();
};
