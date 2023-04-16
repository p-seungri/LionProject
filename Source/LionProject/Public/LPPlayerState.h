// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "GameFramework/PlayerState.h"
#include "LPPlayerState.generated.h"

/**
 * 
 */
enum class ELPSkills : uint8;
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPIsChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelIsChangedDelegate);

UCLASS()
class LIONPROJECT_API ALPPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ALPPlayerState();

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable)
	float GetHPRatio() const;

	UFUNCTION(BlueprintCallable)
	float GetExpRatio() const;


	UFUNCTION(BlueprintCallable)
	float GetScale() const;

	float GetCurrentHP() const;
	float GetHPCurePortion() const;
	float GetAttack(ELPSkills Skill);

	int32 GetDropExp() const;

	bool AddExp(float IncomeExp);
	void SetCurrentExp(float NewExp);

	UFUNCTION(BlueprintCallable)
	void SetCharacterLevel(int32 NewCharacterLevel);

	void SetDamage(float Damage);
	void SetHP(float NewHP);

	void InitPlayerData();
	void SavePlayerData();

	FString SaveSlotName;

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPIsChangedDelegate OnHPIsChanged;
	FOnLevelIsChangedDelegate OnLevelIsChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	float Exp;

	UPROPERTY(Transient)
	float CurrentHP;

	UPROPERTY(Transient)
	float AttackRatio;

	UPROPERTY(Transient)
	float Scale;

private:
	struct FLPCharacterData* CurrentStatData;
	struct FLPSkillData* SkillData;


};
