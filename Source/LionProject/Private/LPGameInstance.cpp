// Fill out your copyright notice in the Description page of Project Settings.


#include "LPGameInstance.h"

ULPGameInstance::ULPGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LPCHARACTER(TEXT("/Game/LPData/LevelData.LevelData"));

	if (DT_LPCHARACTER.Succeeded())
	{
		LPCharacterTable = DT_LPCHARACTER.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LPSKILL(TEXT("/Game/LPData/SkillData.SkillData"));

	if (DT_LPSKILL.Succeeded())
	{
		LPSkillTable = DT_LPSKILL.Object;
	}
	
}

FLPCharacterData* ULPGameInstance::GetLPCharacterData(int32 Level)
{
	return LPCharacterTable->FindRow<FLPCharacterData>(*FString::FromInt(Level), TEXT(""));
}

FLPSkillData* ULPGameInstance::GetLPSkillData(ELPSkills Skill)
{
	return LPSkillTable->FindRow<FLPSkillData>(*FString::FromInt((int8)Skill), TEXT(""));
}
