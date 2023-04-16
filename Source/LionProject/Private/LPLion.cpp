// Fill out your copyright notice in the Description page of Project Settings.


#include "LPLion.h"
#include "LPLionAnimInstance.h"
#include <LPPlayerState.h>
#include <LPGameInstance.h>
#include <LPAIController.h>
#include <DrawDebugHelpers.h>
#include <LPPlayerController.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>

ALPLion::ALPLion()
{
	GetMesh()->SetRelativeLocation(FVector(0.0f, -15.0f, -100.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	static ConstructorHelpers::FObjectFinder<UPhysicsAsset> PA_LION(TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Meshes/SK_Lion_LOD0_PhysicsAsset.SK_Lion_LOD0_PhysicsAsset"));

	if (PA_LION.Succeeded())
	{
		GetMesh()->SetPhysicsAsset(PA_LION.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_LION(TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Meshes/SK_Lion_LOD0.SK_Lion_LOD0"));

	if (SK_LION.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_LION.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> LION_ANIM(TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Animations/LPLionAnimBP.LPLionAnimBP_C"));

	if (LION_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(LION_ANIM.Class);
	}

	BiteRadius = 100.0f;
	BiteRange = 200.0f;

	ClawRadius = 100.0f;
	ClawRange = 200.0f;
	
	CopyRadius = 200.0f;
	CopyRange = 400.0f;

	HowlRadius = 500.0f;
	HowlRange = 500.0f;

	FireRadius = 200.0f;
	FireRange = 500.0f;

	AnimalType = EAnimalType::Lion;

}

void ALPLion::BeginPlay()
{
	Super::BeginPlay();

	if (ActorHasTag(TEXT("Copy")))
	{
		GetCapsuleComponent()->SetRelativeScale3D(FVector(ALPAnimal::PlayerScale));
		GetMesh()->SetRelativeScale3D(FVector(ALPAnimal::PlayerScale));
	}

	LionAnim->OnBiteCheck.AddLambda([this]() -> void
		{
			Attack(ELPSkills::LionBite);
		});
	LionAnim->OnClawCheck.AddLambda([this]() -> void
		{
			Attack(ELPSkills::LionClaw);
		});
	LionAnim->OnCopyCheck.AddLambda([this]() -> void
		{
			Attack(ELPSkills::LionCopy);
		});
	LionAnim->OnHowlCheck.AddLambda([this]() -> void
		{
			Attack(ELPSkills::LionHowl);
		});
	LionAnim->OnFireCheck.AddLambda([this]() -> void
		{
			Attack(ELPSkills::LionFire);
		});

	AttackInitialize();

	if (!IsPlayerControlled())
	{
		auto LPPlayerState = GetLPPlayerState();

		if (LPPlayerState != nullptr)
		{
			LPPlayerState->SetCharacterLevel(5);
		}
	}
}

void ALPLion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LionAnim = Cast<ULPLionAnimInstance>(GetMesh()->GetAnimInstance());


}

float ALPLion::GetBiteRange() const
{
	return BiteRange;
}

float ALPLion::GetClawRange() const
{
	return ClawRange;
}

float ALPLion::GetCopyRange() const
{
	return CopyRange;
}

float ALPLion::GetHowlRange() const
{
	return HowlRange;
}

float ALPLion::GetFireRange() const
{
	return FireRange;
}

void ALPLion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALPLion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Bite"), EInputEvent::IE_Pressed, this, &ALPLion::BiteStart);
	PlayerInputComponent->BindAction(TEXT("Bite"), EInputEvent::IE_Released, this, &ALPLion::BiteEnd);

	PlayerInputComponent->BindAction(TEXT("Claw"), EInputEvent::IE_Pressed, this, &ALPLion::ClawStart);
	PlayerInputComponent->BindAction(TEXT("Claw"), EInputEvent::IE_Released, this, &ALPLion::ClawEnd);

	PlayerInputComponent->BindAction(TEXT("Copy"), EInputEvent::IE_Pressed, this, &ALPLion::CopyStart);
	PlayerInputComponent->BindAction(TEXT("Copy"), EInputEvent::IE_Released, this, &ALPLion::CopyEnd);

	PlayerInputComponent->BindAction(TEXT("Howl"), EInputEvent::IE_Pressed, this, &ALPLion::HowlStart);
	PlayerInputComponent->BindAction(TEXT("Howl"), EInputEvent::IE_Released, this, &ALPLion::HowlEnd);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ALPLion::FireStart);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ALPLion::FireEnd);

	PlayerInputComponent->BindAction(TEXT("LevelUp"), EInputEvent::IE_Pressed, this, &ALPLion::LevelUp);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ALPLion::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ALPLion::MoveRight);

}

void ALPLion::AttackInitialize()
{
	Super::AttackInitialize();

	auto LPPlayerState = GetLPPlayerState();

	if (LPPlayerState != nullptr)
	{
		BiteRadius = 100.0f * LPPlayerState->GetScale();
		BiteRange = 200.0f * LPPlayerState->GetScale();;

		ClawRadius = 100.0f * LPPlayerState->GetScale();
		ClawRange = 200.0f * LPPlayerState->GetScale();

		CopyRadius = 200.0f * LPPlayerState->GetScale();
		CopyRange = 400.0f * LPPlayerState->GetScale();

		HowlRadius = 500.0f * LPPlayerState->GetScale();
		HowlRange = 500.0f * LPPlayerState->GetScale();

		FireRadius = 200.0f * LPPlayerState->GetScale();
		FireRange = 500.0f * LPPlayerState->GetScale();
	}



	BiteEnd();
	ClawEnd();
	CopyEnd();
	HowlEnd();
	FireEnd();
}

void ALPLion::MoveForward(float AxisValue)
{
	Super::MoveForward(AxisValue);
}

void ALPLion::MoveRight(float AxisValue)
{
	Super::MoveRight(AxisValue);
}

void ALPLion::Attack(ELPSkills Skill)
{
	//Super::Attack(Skill);

	if (LionAnim->GetIsDead()) return;

	switch (Skill)
	{
	case ELPSkills::LionBite:
		BiteStart();
		BiteCheck();
		break;
	case ELPSkills::LionClaw:
		ClawStart();
		ClawCheck();
		break;
	case ELPSkills::LionCopy:
		CopyStart();
		CopyCheck();
		break;
	case ELPSkills::LionHowl:
		HowlStart();
		HowlCheck();
		break;
	case ELPSkills::LionFire:
		FireStart();
		FireCheck();
	}

}

void ALPLion::Jump()
{
	if (!GetIsRest())
	{
		Super::Jump();
	}
}

void ALPLion::LevelUp()
{
	auto LPPlayerState = GetLPPlayerState();

	if (LPPlayerState != nullptr)
	{
		LPPlayerState->SetCharacterLevel(FMath::Clamp<int32>(LPPlayerState->GetCharacterLevel() + 1, 1, 10));
	}

	
}

void ALPLion::BiteStart()
{
	auto LPPlayerState = GetLPPlayerState();
	static bool flag = true;
	if (LPPlayerState != nullptr && flag && !LionAnim->GetIsAttacking() && LPPlayerState->GetCharacterLevel() >= 3)
	{
		flag = false;

		LionAnim->OnAttackEnd.Clear();

		FTimerHandle CoolTimeHandle;

		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				flag = true;
			}), 0.1f, false);

		LionAnim->SetIsAttacking(true);
		LionAnim->SetBiteAttack(true);
		LionAnim->OnAttackEnd.AddUObject(this, &ALPLion::AttackInitialize);
		//GetWorld()->GetTimerManager().SetTimer(AttackEndTimer, this, &ALPLion::AttackInitialize, 0.1f, false);
	}

}

void ALPLion::BiteEnd()
{
	
	LionAnim->SetIsAttacking(false);
	LionAnim->SetBiteAttack(false);
}


void ALPLion::BiteCheck()
{
	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + GetActorForwardVector() * BiteRange,
		GetActorLocation() + GetActorForwardVector() * BiteRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(BiteRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * BiteRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = BiteRange * 0.5f + BiteRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, BiteRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			LPLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			
			auto LPAnimal = Cast<ALPAnimal>(HitResult.Actor);

			if (LPAnimal != nullptr && LPPlayerState != nullptr && LPAnimal->ActorHasTag(TEXT("Enemy")) && LPAnimal->GetController() != GetController())
			{
				if (!IsPlayerControlled() && !LPAnimal->IsPlayerControlled())
				{

				}
				else
				{
					FDamageEvent DamageEvent;
					HitResult.Actor->TakeDamage(LPPlayerState->GetAttack(ELPSkills::LionBite), DamageEvent, GetController(), this);
				}
			}
		}
	}
}


void ALPLion::ClawStart()
{
	auto LPPlayerState = GetLPPlayerState();
	static bool flag = true;
	if (flag && !LionAnim->GetIsAttacking() && LPPlayerState->GetCharacterLevel() >= 3)
	{
		flag = false;

		FTimerHandle CoolTimeHandle;

		LionAnim->OnAttackEnd.Clear();

		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				flag = true;
			}), 0.1f, false);

		LionAnim->SetIsAttacking(true);
		LionAnim->SetClawAttack(true);

		LionAnim->OnAttackEnd.AddUObject(this, &ALPLion::AttackInitialize);
		//GetWorld()->GetTimerManager().SetTimer(AttackEndTimer, this, &ALPLion::AttackInitialize, 0.1f, false);
	}
}

void ALPLion::ClawEnd()
{
	LionAnim->SetIsAttacking(false);
	LionAnim->SetClawAttack(false);
}

void ALPLion::ClawCheck()
{
	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation() + GetActorForwardVector() * ClawRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(ClawRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());

			if (LPAnimal && !LPAnimal->GetController()->IsPlayerController())
			{
				DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * ClawRange, ClawRadius, 16, FColor::Green, false, 0.2f);
			}
		}
	}
#endif

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());

			if (LPAnimal && LPPlayerState && LPAnimal->ActorHasTag(TEXT("Enemy")) && LPAnimal->GetController() != GetController())
			{
				LPLOG(Warning, TEXT("Hit Actor Name : %s"), *OverlapResult.Actor->GetName());
				if (!IsPlayerControlled() && !LPAnimal->IsPlayerControlled())
				{

				}
				else
				{
					FDamageEvent DamageEvent;
					OverlapResult.Actor->TakeDamage(LPPlayerState->GetAttack(ELPSkills::LionClaw), DamageEvent, GetController(), this);
				}
			}

		}

	}
}

void ALPLion::CopyStart()
{
	auto LPPlayerState = GetLPPlayerState();
	static bool flag = true;

	if (LPPlayerState && flag && !LionAnim->GetIsAttacking() && LPPlayerState->GetCharacterLevel() >= 5)
	{
		LionAnim->OnAttackEnd.Clear();

		flag = false;

		FTimerHandle CoolTimeHandle;

		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				flag = true;
			}), 3.0f, false);

		static auto LION_ANIM = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Animations/LPLionAnimBP.LPLionAnimBP_C"));
		
		for (int32 i = -2; i <= 2; i++)
		{
			if (i == 0) continue;

			auto SpawnedActor = GetWorld()->SpawnActor<ALPLion>(GetActorLocation() + FVector(50.0f * FMath::Abs(i), 100.0f * i, 0.0f), GetActorRotation());

			if (SpawnedActor != nullptr)
			{
				SpawnedActor->Tags.Add(TEXT("Copy"));

				SpawnedActor->GetMesh()->SetRelativeScale3D(FVector(LPPlayerState->GetScale()));

				SpawnedActor->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

				if (LION_ANIM != nullptr)
				{
					SpawnedActor->GetMesh()->SetAnimClass(LION_ANIM);
					SpawnedActor->LionAnim->SetIsAttacking(true);
					SpawnedActor->LionAnim->SetCopyAttack(true);
				}

				SpawnedActor->Tags.Add(*GetName());

				SpawnedActor->LionAnim->OnAttackEnd.AddLambda([this, SpawnedActor]() -> void
					{
						CopyEnd();
						SpawnedActor->CopyEnd();
						SpawnedActor->Destroy();
					});
			}
		}

		LionAnim->OnAttackEnd.AddUObject(this, &ALPLion::AttackInitialize);
		//GetWorld()->GetTimerManager().SetTimer(AttackEndTimer, this, &ALPLion::AttackInitialize, 0.1f, false);
	}
}


void ALPLion::CopyCheck()
{
	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation() + GetActorForwardVector() * CopyRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(CopyRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());

			if (LPAnimal && !LPAnimal->GetController()->IsPlayerController())
			{
				DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * CopyRange, CopyRadius, 16, FColor::Green, false, 0.2f);
			}
		}
	}
#endif

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());
			

			if (LPAnimal && LPPlayerState && LPAnimal->ActorHasTag(TEXT("Enemy")) && !ActorHasTag(*LPAnimal->GetName()))
			{
				LPLOG(Warning, TEXT("Hit Actor Name : %s"), *OverlapResult.Actor->GetName());

				FDamageEvent DamageEvent;
				OverlapResult.Actor->TakeDamage(LPPlayerState->GetAttack(ELPSkills::LionCopy), DamageEvent, GetController(), this);
			}

		}

	}
}

void ALPLion::CopyEnd()
{
	LionAnim->SetIsAttacking(false);
	LionAnim->SetCopyAttack(false);
}

void ALPLion::HowlStart()
{
	auto LPPlayerState = GetLPPlayerState();
	static bool flag = true;
	if (LPPlayerState && flag && !LionAnim->GetIsAttacking() && LPPlayerState->GetCharacterLevel() >= 7)
	{
		LionAnim->OnAttackEnd.Clear();

		flag = false;

		FTimerHandle CoolTimeHandle;

		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				flag = true;
			}), 5.0f, false);

		LionAnim->SetIsAttacking(true);
		LionAnim->SetHowlAttack(true);
		LionAnim->OnAttackEnd.AddUObject(this, &ALPLion::AttackInitialize);
		//GetWorld()->GetTimerManager().SetTimer(AttackEndTimer, this, &ALPLion::AttackInitialize, 0.1f, false);
	}
}

void ALPLion::HowlCheck()
{
	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	static UParticleSystem* StunAsset = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/StarterContent/Particles/P_Sparks.P_Sparks"));

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation() + GetActorForwardVector() * HowlRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(HowlRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());

			if (LPAnimal && !LPAnimal->GetController()->IsPlayerController())
			{
				DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * HowlRange, HowlRadius, 16, FColor::Green, false, 0.2f);
			}
		}
	}
#endif

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());

			if (LPPlayerState && LPAnimal && LPAnimal->ActorHasTag(TEXT("Enemy")) && LPAnimal->GetController() != GetController())
			{
				LPLOG(Warning, TEXT("Hit Actor Name : %s"), *OverlapResult.Actor->GetName());


				auto SpawnedEmitter = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), 
					StunAsset, 
					LPAnimal->GetMesh()->GetSocketLocation(TEXT("StunSocket")),
					LPAnimal->GetActorRotation(), 
					false);

				

				FTimerHandle RestartHandle;

				if (SpawnedEmitter != nullptr)
				{
					SpawnedEmitter->SetRelativeScale3D(FVector(3.0f));

					if (!LPAnimal->GetController()->IsPlayerController())
					{
						LPAnimal->SetIsPatrol(false);
						LPAnimal->SetIsAttack(false);

						GetWorld()->GetTimerManager().SetTimer(RestartHandle, FTimerDelegate::CreateLambda([this, LPAnimal, SpawnedEmitter]() -> void
							{
								LPAnimal->SetIsPatrol(true);
								LPAnimal->SetIsAttack(true);
								SpawnedEmitter->DestroyComponent();
							}), 5.0f, false);

						if (!IsPlayerControlled() && !LPAnimal->IsPlayerControlled())
						{

						}
						else
						{
							FDamageEvent DamageEvent;
							OverlapResult.Actor->TakeDamage(LPPlayerState->GetAttack(ELPSkills::LionHowl), DamageEvent, GetController(), this);
						}
					}
					else
					{
						auto LPPlayerController = Cast<ALPPlayerController>(LPAnimal->GetController());
						DisableInput(LPPlayerController);

						GetWorld()->GetTimerManager().SetTimer(RestartHandle, FTimerDelegate::CreateLambda([this, LPPlayerController, SpawnedEmitter]() -> void
							{
								EnableInput(LPPlayerController);
								SpawnedEmitter->DestroyComponent();
							}), 5.0f, false);

						FDamageEvent DamageEvent;
						OverlapResult.Actor->TakeDamage(LPPlayerState->GetAttack(ELPSkills::LionHowl), DamageEvent, GetController(), this);
					}
				}
			}
		}
	}
}

void ALPLion::HowlEnd()
{
	LionAnim->SetIsAttacking(false);
	LionAnim->SetHowlAttack(false);
}

void ALPLion::FireStart()
{
	auto LPPlayerState = GetLPPlayerState();
	static bool flag = true;
	
	if (LPPlayerState && flag && !LionAnim->GetIsAttacking() && LPPlayerState->GetCharacterLevel() >= 9)
	{
		LionAnim->OnAttackEnd.Clear();

		flag = false;

		FTimerHandle CoolTimeHandle;

		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				flag = true;
			}), 8.0f, false);

		LionAnim->SetIsAttacking(true);
		LionAnim->SetFireAttack(true);

		LionAnim->OnAttackEnd.AddUObject(this, &ALPLion::AttackInitialize);
		//GetWorld()->GetTimerManager().SetTimer(AttackEndTimer, this, &ALPLion::AttackInitialize, 0.1f, false);
	}
}

void ALPLion::FireCheck()
{
	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	static UParticleSystem* FireAsset = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));

	for (int32 i = 2; i < 7; i++)
	{
		auto SpawnedEmitter = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			FireAsset,
			GetActorLocation() + GetActorForwardVector() * 100.0f * i,
			GetActorRotation(),
			FVector(2.0f * GetMesh()->GetRelativeScale3D().X),
			false);

		if (SpawnedEmitter != nullptr)
		{
			FTimerHandle EmitterHandle;

			GetWorld()->GetTimerManager().SetTimer(EmitterHandle, FTimerDelegate::CreateLambda([this, SpawnedEmitter]() -> void
				{
					SpawnedEmitter->DestroyComponent();
				}), 2.0f, false);
		}

	}



	TArray<FHitResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel(
		OverlapResults,
		GetActorLocation() + 100.0f,
		GetActorLocation() + GetActorForwardVector() * FireRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(FireRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FireRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FireRange * 0.5f + FireRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, FireRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			auto LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());

			if (LPPlayerState && LPAnimal && LPAnimal->ActorHasTag(TEXT("Enemy")) && LPAnimal->GetController() != GetController())
			{
				LPLOG(Warning, TEXT("Hit Actor Name : %s"), *OverlapResult.Actor->GetName());
				if (!IsPlayerControlled() && !LPAnimal->IsPlayerControlled())
				{

				}
				else
				{
					FDamageEvent DamageEvent;
					OverlapResult.Actor->TakeDamage(LPPlayerState->GetAttack(ELPSkills::LionFire), DamageEvent, GetController(), this);
				}
			}

		}

	}
}

void ALPLion::FireEnd()
{
	LionAnim->SetIsAttacking(false);
	LionAnim->SetFireAttack(false);
}
