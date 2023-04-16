// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ELPSkills : uint8
{
	LionBite = 0, // Lion Bite
	LionClaw, //Lion Claw
	LionCopy,
	LionHowl,
	LionFire,
	Skill6,
	Skill7,
	Others

};

UENUM(BlueprintType)
enum class EAnimalType : uint8
{
	Lion = 0,
	Zebra,
	Elephant,
	Crocodile,
	Hippopotamus,
	Rhinoceros
};

UENUM()
enum class EControlMode : uint8
{
	Normal,
	Runaway
};

DECLARE_LOG_CATEGORY_EXTERN(LionProject, Log, All);
#define LPLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LPLOG_S(Verbosity) UE_LOG(LionProject, Verbosity, TEXT("%s"), *LPLOG_CALLINFO)
#define LPLOG(Verbosity, Format, ...) UE_LOG(LionProject, Verbosity, TEXT("%s %s"), *LPLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))