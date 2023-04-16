// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "LPAnimal.h"
#include "LPLion.generated.h"

/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPLion : public ALPAnimal
{
	GENERATED_BODY()
public:
	ALPLion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void AttackInitialize() override;

	float GetBiteRange() const;
	float GetClawRange() const;
	float GetCopyRange() const;
	float GetHowlRange() const;
	float GetFireRange() const;

	UPROPERTY()
	class ULPLionAnimInstance* LionAnim;
	

private:
	virtual void MoveForward(float AxisValue) override;
	virtual void MoveRight(float AxisValue) override;
	virtual void Attack(ELPSkills Skill) override;
	virtual void Jump() override;
	void LevelUp();

	void BiteStart();
	void BiteEnd();
	void BiteCheck();

	void ClawStart();
	void ClawEnd();
	void ClawCheck();

	void CopyStart();
	void CopyCheck();
	void CopyEnd();

	void HowlStart();
	void HowlCheck();
	void HowlEnd();

	void FireStart();
	void FireCheck();
	void FireEnd();


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float BiteRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float BiteRange;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float ClawRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float ClawRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float CopyRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float CopyRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float HowlRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float HowlRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float FireRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float FireRange;
};