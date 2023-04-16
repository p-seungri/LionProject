// Fill out your copyright notice in the Description page of Project Settings.


#include "LPLionBaby.h"
#include "Engine/TargetPoint.h"
#include <EngineUtils.h>
#include <LPAIController.h>
#include <LPPlayerState.h>
#include <LPLionAnimInstance.h>
#include <LPPlayerController.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Engine/BlueprintGeneratedClass.h"

ALPLionBaby::ALPLionBaby()
{
	GetMesh()->SetRelativeScale3D(FVector(0.5f));
}

void ALPLionBaby::BeginPlay()
{
	Super::BeginPlay();

	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	if (LPPlayerState != nullptr)
	{
		if (IsPlayerControlled())
		{

			Tags.Add(TEXT("Player"));
			Tags.Add(TEXT("Enemy"));

			for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
			{
				if (It->ActorHasTag("Stage01"))
				{
					LPPlayerState->SetCharacterLevel(1);
					LPPlayerState->SetCurrentExp(0.0f);
				}
			}
		}

		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag("Stage02"))
			{
				LPPlayerState->SetCharacterLevel(3);
				LPPlayerState->SetCurrentExp(0.0f);
			}
		}

		USkeletalMesh* LionAsset = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Meshes/SK_Lion_LOD0.SK_Lion_LOD0"));
		if (LPPlayerState->GetCharacterLevel() >= 5)
		{
			GetMesh()->SetSkeletalMesh(LionAsset);
		}
		LPPlayerState->OnLevelIsChanged.AddLambda([this, LPPlayerState]() -> void
			{
				USkeletalMesh* LionAsset = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Meshes/SK_Lion_LOD0.SK_Lion_LOD0"));
				if (LPPlayerState->GetCharacterLevel() >= 5)
				{
					GetMesh()->SetSkeletalMesh(LionAsset);
				}
			});

		LPPlayerState->OnHPIsZero.AddUObject(this, &ALPLionBaby::OnDestory);
	}


	
	OnActorBeginOverlap.AddDynamic(this, &ALPLionBaby::OnPrideOverlapBegin);
	OnActorBeginOverlap.AddDynamic(this, &ALPLionBaby::OnRunawayOverlapBegin);
	OnActorBeginOverlap.AddDynamic(this, &ALPLionBaby::OnHuntOverlapBegin);
	OnActorBeginOverlap.AddDynamic(this, &ALPLionBaby::OnBossStageOverlapBegin);
}

void ALPLionBaby::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void ALPLionBaby::OnPrideOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Pride")))
	{
		FTimerHandle DelayHandle1, DelayHandle2;
		float DelayTime = 17.0f;

		auto LPPlayerController = Cast<ALPPlayerController>(GetController());

		if (LPPlayerController != nullptr)
		{
			DisableInput(LPPlayerController);

			DirectionToMove.X = 0.0f;
			DirectionToMove.Y = 0.0f;

			AnimalAnim->SetIsSprint(false);


			GetWorld()->GetTimerManager().SetTimer(DelayHandle1, FTimerDelegate::CreateUObject(this, &ALPLionBaby::OnFatherDeath), DelayTime, false);

			GetWorld()->GetTimerManager().SetTimer(DelayHandle2, FTimerDelegate::CreateLambda([this, LPPlayerController]() -> void
				{
					for (TActorIterator<ALPLion> It(GetWorld()); It; ++It)
					{
						if (It->ActorHasTag(TEXT("NewFather")))
						{
							auto LPAIController = Cast<ALPAIController>(It->GetController());

							if (LPAIController != nullptr)
							{
								LPAIController->StopAI();
								LPAIController->RunAI();
							}
						}
					}
					EnableInput(LPPlayerController);
				}), DelayTime + 3.0f, false);

		}


		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag(TEXT("Pride")))
			{
				auto NewLocation = It->GetActorLocation();
				SetActorLocation(NewLocation);
				break;
			}
			
		}
		
		for (TActorIterator<ALPLion> It(GetWorld()); It; ++It)
		{
			UAnimationAsset* ClawAnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Animations/ANIM_Lion_ClawsAttack.ANIM_Lion_ClawsAttack"));
			if (It->ActorHasTag(TEXT("Father")))
			{
				It->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
				
				if (ClawAnimAsset != nullptr)
				{
					It->GetMesh()->PlayAnimation(ClawAnimAsset, true);
				}
			}

			if (It->ActorHasTag(TEXT("NewFather")))
			{
				It->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

				if (ClawAnimAsset != nullptr)
				{
					It->GetMesh()->PlayAnimation(ClawAnimAsset, true);
				}
			}

		}
	}



}

void ALPLionBaby::OnRunawayOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Runaway")))
	{
		SetControlMode(EControlMode::Runaway);
		
		DirectionToMove.X = 0.0f;
		DirectionToMove.Y = 0.0f;

		AnimalAnim->SetIsSprint(false);

		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag(TEXT("RunawayPoint")))
			{
				auto NewLocation = It->GetActorLocation();
				SetActorLocation(NewLocation);
				break;
			}

		}
		
	}
}

void ALPLionBaby::OnHuntOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Hunt")))
	{
		auto LPPlayerController = Cast<ALPPlayerController>(GetController());
		
		if (LPPlayerController != nullptr)
		{
			DisableInput(LPPlayerController);

			FTimerHandle DelayTimer;

			DirectionToMove.X = 0.0f;
			DirectionToMove.Y = 0.0f;

			AnimalAnim->SetIsSprint(false);

			GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([this, LPPlayerController]() -> void
				{
					EnableInput(LPPlayerController);
				}), 5.0f, false);
		}
	}
}

void ALPLionBaby::OnBossStageOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("BossStage"))
	{
		auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

		if (LPPlayerState != nullptr)
		{
			LPPlayerState->SavePlayerData();
		}
		
	}
}

void ALPLionBaby::OnDestory()
{
	if (IsPlayerControlled())
	{
		auto LPPlayerController = Cast<ALPPlayerController>(GetController());

		if (LPPlayerController != nullptr)
		{
			LPPlayerController->SetGameOverWidget();
		}
	}
	
	
}

void ALPLionBaby::OnFatherDeath()
{
	static auto LION_ANIM = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Animations/LPLionAnimBP.LPLionAnimBP_C"));

	for (TActorIterator<ALPLion> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(TEXT("Father")))
		{
			UAnimationAsset* DeadAnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Animations/ANIM_Lion_Death.ANIM_Lion_Death"));
			It->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

			if (DeadAnimAsset != nullptr)
			{
				FTimerHandle DeadTimer;
				It->GetMesh()->PlayAnimation(DeadAnimAsset, false);
				GetWorld()->GetTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([this, It]() -> void
					{
						auto LPLion = Cast<ALPLion>(*It);

						if (LPLion != nullptr)
						{
							LPLion->Destroy();
						}
						
					}), 3.0f, false);
			}
			
		}

		if (It->ActorHasTag(TEXT("NewFather")))
		{
			It->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			It->GetMesh()->SetAnimInstanceClass(LION_ANIM);
			It->SetIsAttack(true);
			It->SetIsPatrol(true);

		}
	}
}
