// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "Animation/AnimInstance.h"
#include "LPAnimalAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHitEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRestStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRestEndDelegate);

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ULPAnimalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	ULPAnimalAnimInstance();
	
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void SetIsSprint(bool Sprint);
	void SetDeadAnim() { IsDead = true; }
	void SetIsHit(bool Hit);
	void SetIsAttacking(bool Attacking);
	void SetIsRest(bool Rest);
	bool GetIsRest() const;
	bool GetIsDead() const;
	bool GetIsAttacking() const;


	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_AttackCheck();

	UFUNCTION()
	void AnimNotify_HitEnd();

	FOnAttackCheckDelegate OnAttackCheck;
	FOnAttackEndDelegate OnAttackEnd;
	FOnHitEndDelegate OnHitEnd;
	FOnRestStartDelegate OnRestStart;
	FOnRestEndDelegate OnRestEnd;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsSprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsHit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsRest;
};
