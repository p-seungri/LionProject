// Fill out your copyright notice in the Description page of Project Settings.


#include "LPPlayerState.h"
#include <LPGameInstance.h>
#include <LPGameModeBase.h>
#include <LPGameState.h>
#include <LPSaveGame.h>

ALPPlayerState::ALPPlayerState()
{
	CharacterLevel = 1;
	Exp = 0.0f;
	CurrentHP = 0.0f;
	AttackRatio = 0.0f;
	Scale = 1.0f;
	SaveSlotName = TEXT("Player0");

}

void ALPPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CurrentStatData = Cast<ULPGameInstance>(GetGameInstance())->GetLPCharacterData(CharacterLevel);

	if (CurrentStatData != nullptr)
	{
		SetHP(CurrentStatData->MaxHP);
		AttackRatio = CurrentStatData->AttackRatio;
	}

}

int32 ALPPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

float ALPPlayerState::GetCurrentHP() const
{
	return CurrentHP;
}

float ALPPlayerState::GetHPCurePortion() const
{
	return CurrentStatData->MaxHP / 10.0f;
}

float ALPPlayerState::GetHPRatio() const
{
	if (CurrentHP <= KINDA_SMALL_NUMBER)
		return 0.0f;
	
	float Result = CurrentHP / CurrentStatData->MaxHP;
	return Result;
}

float ALPPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	return Result;
}

float ALPPlayerState::GetAttack(ELPSkills Skill)
{
	auto ABGameInstance = Cast<ULPGameInstance>(GetGameInstance());

	if (ABGameInstance != nullptr)
	{
		SkillData = ABGameInstance->GetLPSkillData(Skill);
		return AttackRatio * SkillData->Attack;
	}
	else
	{
		return 10.0f;
	}
}

bool ALPPlayerState::AddExp(float IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	Exp += IncomeExp;

	LPLOG(Warning, TEXT("Get %d Exp!"), IncomeExp);

	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		LPLOG(Warning, TEXT("%d Level Achieved"), CharacterLevel);
		DidLevelUp = true;
	}

	return DidLevelUp;
}

void ALPPlayerState::SetCurrentExp(float NewExp)
{
	Exp = NewExp;
}



float ALPPlayerState::GetScale() const
{
	return Scale;
}

void ALPPlayerState::SetDamage(float Damage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - Damage, 0, CurrentStatData->MaxHP));

	OnHPIsChanged.Broadcast();
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		OnHPIsZero.Broadcast();
	}
}

void ALPPlayerState::SetHP(float NewHP)
{
	CurrentHP = NewHP;
}

void ALPPlayerState::InitPlayerData()
{
	auto LPSaveGame = Cast<ULPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	if (nullptr == LPSaveGame)
	{
		LPSaveGame = GetMutableDefault<ULPSaveGame>();
	}

	SetCharacterLevel(LPSaveGame->Level);
	Exp = LPSaveGame->Exp;
	
	SavePlayerData();

}

int32 ALPPlayerState::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

void ALPPlayerState::SavePlayerData()
{
	ULPSaveGame* NewPlayerData = NewObject<ULPSaveGame>();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;

	UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0);
}

void ALPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	CurrentStatData = Cast<ULPGameInstance>(GetGameInstance())->GetLPCharacterData(CharacterLevel);

	if (CurrentStatData != nullptr)
	{
		SetHP(CurrentStatData->MaxHP);
		AttackRatio = CurrentStatData->AttackRatio;
	}
}


void ALPPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto ABGameInstance = Cast<ULPGameInstance>(GetGameInstance());

	if (ABGameInstance != nullptr)
	{
		CurrentStatData = ABGameInstance->GetLPCharacterData(NewCharacterLevel);

		CharacterLevel = NewCharacterLevel;

		SetHP(CurrentStatData->MaxHP);
		Scale = CurrentStatData->Scale;
		OnLevelIsChanged.Broadcast();
		AttackRatio = CurrentStatData->AttackRatio;
	}

}
