// Fill out your copyright notice in the Description page of Project Settings.


#include "LPRhino.h"
#include <LPLionBaby.h>
#include <EngineUtils.h>
#include <LPPlayerState.h>
#include <LPAIController.h>
#include <LPGameState.h>
#include <Engine/TargetPoint.h>

ALPRhino::ALPRhino()
{
	GetMesh()->SetRelativeLocation(FVector(0.0f, -15.0f, -100.0f));

	static ConstructorHelpers::FObjectFinder<UPhysicsAsset> PA_RHINO(TEXT("/Game/AfricanAnimalsPack/Rhinoceros/Meshes/SK_Rhinoceros_PhysicsAsset.SK_Rhinoceros_PhysicsAsset"));

	if (PA_RHINO.Succeeded())
	{
		GetMesh()->SetPhysicsAsset(PA_RHINO.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_RHINO(TEXT("/Game/AfricanAnimalsPack/Rhinoceros/Meshes/SK_Rhinoceros.SK_Rhinoceros"));

	if (SK_RHINO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_RHINO.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	

	static ConstructorHelpers::FClassFinder<UAnimInstance> RHINO_ANIM(TEXT("/Game/AfricanAnimalsPack/Rhinoceros/Animations/LPRhinoAnimBP.LPRhinoAnimBP_C"));

	if (RHINO_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(RHINO_ANIM.Class);
	}

	CurrentSpeed = 4800.0f;
	MovementDirection = FVector(1.0f, 0.0f, 0.0f);

	AnimalType = EAnimalType::Rhinoceros;

}

void ALPRhino::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(1.5f));
	
}

void ALPRhino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(MovementDirection * (CurrentSpeed * DeltaTime));

}

void ALPRhino::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnActorBeginOverlap.AddDynamic(this, &ALPRhino::OnPlayerOverlapBegin);
	OnDestroyed.AddDynamic(this, &ALPRhino::OnDestroy);
}

void ALPRhino::OnPlayerOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		auto LPLionBaby = Cast<ALPLionBaby>(OtherActor);

		if (LPLionBaby != nullptr)
		{
			FDamageEvent DamageEvent;
			LPLionBaby->TakeDamage(20.0f, DamageEvent, GetController(), this);
		}

	}
}

void ALPRhino::OnDestroy(AActor* DestroyedActor)
{
	static bool flag = true;
	for (TActorIterator<ALPLionBaby> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(TEXT("Player")))
		{
			auto LPPlayerState = It->GetLPPlayerState();

			if (nullptr != LPPlayerState)
			{
				LPPlayerState->AddExp(0.5f);

				if (LPPlayerState->GetCharacterLevel() == 3 && flag)
				{
					for (TActorIterator<ATargetPoint> BoxIt(GetWorld()); BoxIt; ++BoxIt)
					{
						if (BoxIt->ActorHasTag(TEXT("SpawnEnd")))
						{
							GetWorld()->SpawnActor<ALPRhino>(BoxIt->GetActorLocation(), FRotator::ZeroRotator);
							flag = false;
							break;
						}
					}

					auto LPGameState = Cast<ALPGameState>(GetWorld()->GetGameState());
				
					FTimerHandle DelayTimer;

					GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([this, LPGameState]() -> void
						{
							if (LPGameState != nullptr)
							{
								LPGameState->SetStageCleared();
							}
							UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage02"));
						}), 5.0f, false);
				}
			}

			
				

		}
	}
}
