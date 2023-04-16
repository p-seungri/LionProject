// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "LPGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FLPCharacterData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLPCharacterData() : Level(1), MaxHP(100.0f), AttackRatio(1.0f), DropExp(10.0f), NextExp(30.0f), Scale(0.5f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float NextExp;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Scale;
};

USTRUCT(BlueprintType)
struct FLPSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLPSkillData() : Skill(ELPSkills::Others), Attack(10.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ELPSkills Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Attack;
};


/**
 * 
 */
UCLASS()
class LIONPROJECT_API ULPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	ULPGameInstance();

	FLPCharacterData* GetLPCharacterData(int32 Level);

	FLPSkillData* GetLPSkillData(ELPSkills Skill);

	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	class UDataTable* LPCharacterTable;

	UPROPERTY()
	class UDataTable* LPSkillTable;
};
