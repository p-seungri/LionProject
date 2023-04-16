// Fill out your copyright notice in the Description page of Project Settings.


#include "LPAnimal.h"
#include <LPAnimalAnimInstance.h>
#include <LPPlayerState.h>
#include <LPAIController.h>
#include <Components/WidgetComponent.h>
#include <LPHPBarWidget.h>
#include <LPPlayerController.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>

float ALPAnimal::PlayerScale = 0.5f;

// Sets default values
ALPAnimal::ALPAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());


	GetMesh()->SetRelativeLocation(FVector(0.0f, -15.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("AnimalPreset"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(100.0f);
	GetCapsuleComponent()->SetCapsuleRadius(60.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AnimalPreset"));


	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	AIControllerClass = ALPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SetPlayerState(NewObject<ALPPlayerState>());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HPBAR(TEXT("/Game/LPUI/UI_HPBar.UI_HPBar_C"));

	if (UI_HPBAR.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HPBAR.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	AnimalType = EAnimalType::Lion;

	SetCanBeDamaged(true);
	IsRest = true;
	IsAttack = false;
	IsPatrol = false;

	SetControlMode(EControlMode::Normal);

	RunawayStart = true;

	AttackRadius = 100.0f;
	AttackRange = 200.0f;
}


// Called when the game starts or when spawned
void ALPAnimal::BeginPlay()
{
	Super::BeginPlay();

	HPBarWidget->SetHiddenInGame(true);
	//GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5f));
	
	SetIsRest(IsRest);

	if (!GetController()->IsPlayerController())
	{
		
		auto LPAIController = Cast<ALPAIController>(GetController());

		if (LPAIController != nullptr)
		{
			LPAIController->RunAI();
			SetIsAttack(IsAttack);
			SetIsPatrol(IsPatrol);
		}
	}
	else
	{
		auto LPPlayerController = Cast<ALPPlayerController>(GetController());

		if (LPPlayerController != nullptr)
		{
			DisableInput(LPPlayerController);

			FTimerHandle DelayTimer;

			GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([this, LPPlayerController]() -> void
				{
					EnableInput(LPPlayerController);
				}), 5.0f, false);
		}

		
	}

	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	if (LPPlayerState != nullptr)
	{

		if (AnimalType != EAnimalType::Elephant)
		{
			GetMesh()->SetRelativeScale3D(FVector(LPPlayerState->GetScale()));

			if (ActorHasTag(TEXT("Copy")))
			{
				GetCapsuleComponent()->SetRelativeScale3D(FVector(ALPAnimal::PlayerScale));
				GetMesh()->SetRelativeScale3D(FVector(ALPAnimal::PlayerScale));
			}
		}

		LPPlayerState->OnHPIsZero.AddUObject(this, &ALPAnimal::OnDeath);
		LPPlayerState->OnHPIsChanged.AddLambda([this]() -> void
			{
				AnimalAnim->SetIsHit(true);
			});
		LPPlayerState->OnLevelIsChanged.AddLambda([this, LPPlayerState]() -> void
			{
				if (AnimalType == EAnimalType::Elephant)
				{

				}
				else
				{
					if (LPPlayerState->GetCharacterLevel() >= 5)
					{
						GetCapsuleComponent()->SetRelativeScale3D(FVector(LPPlayerState->GetScale()));
					}

					GetMesh()->SetRelativeScale3D(FVector(LPPlayerState->GetScale()));

					if (ActorHasTag("Player"))
					{
						ALPAnimal::PlayerScale = LPPlayerState->GetScale();
					}

				}
			});

		if (LPPlayerState->GetCharacterLevel() >= 5)
		{
			GetCapsuleComponent()->SetRelativeScale3D(FVector(LPPlayerState->GetScale()));

			AttackRadius = 100.0f * LPPlayerState->GetScale();
			AttackRange = 200.0f * LPPlayerState->GetScale();;
		}
		else
		{
			AttackRadius = 100.0f;
			AttackRange = 200.0f;
		}


		AttackRadius = 100.0f * LPPlayerState->GetScale();
		AttackRange = 200.0f * LPPlayerState->GetScale();
	}

	auto CharacterWidget = Cast<ULPHPBarWidget>(HPBarWidget->GetUserWidgetObject());

	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindPlayerState(LPPlayerState);
	}


	AnimalAnim->OnAttackCheck.AddLambda([this]() -> void
		{
			Attack(ELPSkills::Others);
		});
	AnimalAnim->OnHitEnd.AddLambda([this]() -> void
		{
			AnimalAnim->SetIsHit(false);
		});
	AnimalAnim->OnRestStart.AddLambda([this]() -> void
		{
			GetWorld()->GetTimerManager().SetTimer(RestHandle, FTimerDelegate::CreateLambda([this]() -> void
				{
					auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

					if (LPPlayerState != nullptr)
					{
						LPPlayerState->SetHP(LPPlayerState->GetCurrentHP() + LPPlayerState->GetHPCurePortion());
					}

				}), 5.0f, true);
		});
	AnimalAnim->OnRestEnd.AddLambda([this]() -> void
		{
			GetWorld()->GetTimerManager().ClearTimer(RestHandle);
		});

}

void ALPAnimal::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ALPAnimal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimalAnim = Cast<ULPAnimalAnimInstance>(GetMesh()->GetAnimInstance());

	




}

// Called every frame
void ALPAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DirectionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}

}

// Called to bind functionality to input
void ALPAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ALPAnimal::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ALPAnimal::SprintEnd);
	PlayerInputComponent->BindAction(TEXT("Rest"), EInputEvent::IE_Released, this, &ALPAnimal::RestToggle);
	PlayerInputComponent->BindAction(TEXT("Invincible"), EInputEvent::IE_Pressed, this, &ALPAnimal::SetInvincible);

}

float ALPAnimal::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CanBeDamaged())
	{
		LPLOG(Warning, TEXT("Actor: %s, Took Damage: %f"), *GetName(), FinalDamage);

		HPBarWidget->SetHiddenInGame(false);

		GetWorld()->GetTimerManager().SetTimer(HPBarTimer, FTimerDelegate::CreateLambda([this]() -> void
			{
				HPBarWidget->SetHiddenInGame(true);
			}), 3.0f, false);

		auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

		if (LPPlayerState != nullptr)
		{
			LPPlayerState->SetDamage(FinalDamage);
		}

		if (AnimalAnim->GetIsDead())
		{
			GetWorld()->GetTimerManager().ClearTimer(UpdateIsAttackTimer);
			
			if (EventInstigator && EventInstigator->IsPlayerController())
			{
				SetActorEnableCollision(false);
				auto LPAIController = Cast<ALPAIController>(GetController());

				if (LPAIController != nullptr)
				{
					LPAIController->GetBlackboardComponent()->SetValueAsBool(ALPAIController::IsDeadKey, AnimalAnim->GetIsDead());
				}

				auto LPPlayerController = Cast<ALPPlayerController>(EventInstigator);

				if (LPPlayerController != nullptr)
				{
					LPPlayerController->NPCKill(this);
				}
			}
		}
	}
	
	
	return FinalDamage;
}

void ALPAnimal::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


}

void ALPAnimal::AttackInitialize()
{
}

bool ALPAnimal::GetIsRest() const
{
	return IsRest;
}

bool ALPAnimal::GetIsAttack() const
{
	return IsAttack;
}

bool ALPAnimal::GetIsPatrol() const
{
	return IsPatrol;
}

void ALPAnimal::SetIsRest(bool Rest)
{
	IsRest = Rest;
	AnimalAnim->SetIsRest(IsRest);
}

void ALPAnimal::SetIsAttack(bool Attack)
{
	IsAttack = Attack;

	if (!AnimalAnim->GetIsDead())
	{
		auto LPAIController = Cast<ALPAIController>(GetController());

		if (LPAIController != nullptr)
		{
			LPAIController->GetBlackboardComponent()->SetValueAsBool(ALPAIController::AttackKey, IsAttack);
		}
	}
}

void ALPAnimal::SetIsPatrol(bool Patrol)
{
	IsPatrol = Patrol;

	if (!AnimalAnim->GetIsDead())
	{
		auto LPAIController = Cast<ALPAIController>(GetController());

		if (LPAIController != nullptr)
		{
			LPAIController->GetBlackboardComponent()->SetValueAsBool(ALPAIController::PatrolKey, IsPatrol);
		}
		
	}

}


ALPPlayerState* ALPAnimal::GetLPPlayerState()
{
	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	if (LPPlayerState != nullptr)
	{
		return LPPlayerState;
	}
	else
	{
		return nullptr;
	}
}

EAnimalType ALPAnimal::GetAnimalType() const
{
	return AnimalType;
}

void ALPAnimal::OnDeath()
{
	AnimalAnim->SetDeadAnim();
	SetActorEnableCollision(false);
	GetWorld()->GetTimerManager().SetTimer(HPBarTimer, FTimerDelegate::CreateLambda([this]() -> void
		{
			Destroy();
		}), 5.0f, false);
}

void ALPAnimal::MoveForward(float AxisValue)
{
	
	if (CurrentControlMode == EControlMode::Normal)
	{
		if (!IsRest)
		{
			DirectionToMove.X = AxisValue;
		}
		else
		{
			DirectionToMove.X = 0.0f;
		}
	}
	else
	{
		if (RunawayStart)
		{
			DirectionToMove.X = -1.0f;
			RunawayStart = false;
			return;
		}

		DirectionToMove.X = 0.0f;
	}
	
	
}

void ALPAnimal::MoveRight(float AxisValue)
{
	if (CurrentControlMode == EControlMode::Normal)
	{
		if (!IsRest)
		{
			DirectionToMove.Y = AxisValue;
		}
		else
		{
			DirectionToMove.Y = 0.0f;
		}
	}
	else
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), AxisValue);
		DirectionToMove.Y = 0.0f;
	}
	
}

void ALPAnimal::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	AnimalAnim->SetIsSprint(true);
}

void ALPAnimal::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	AnimalAnim->SetIsSprint(false);
}

void ALPAnimal::RestToggle()
{
	if (CurrentControlMode == EControlMode::Normal)
	{
		IsRest = !IsRest;
		AnimalAnim->SetIsRest(IsRest);
	}
	
}

void ALPAnimal::SetInvincible()
{
	SetCanBeDamaged(false);
}


void ALPAnimal::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (NewControlMode)
	{
	case EControlMode::Normal:
		SpringArm->TargetArmLength = 800.0f;
		SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
		break;
	case EControlMode::Runaway:
		SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, -180.0f, SpringArm->GetRelativeRotation().Roll));
		break;
	}
}

void ALPAnimal::AttackStart()
{
	static bool flag = true;
	if (flag && !AnimalAnim->GetIsAttacking())
	{
		AnimalAnim->OnAttackEnd.Clear();

		flag = false;

		FTimerHandle CoolTimeHandle;

		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				flag = true;
			}), 0.1f, false);

		AnimalAnim->SetIsAttacking(true);

		AnimalAnim->OnAttackEnd.AddUObject(this, &ALPAnimal::AttackEnd);
		//GetWorld()->GetTimerManager().SetTimer(AttackEndTimer, this, &ALPAnimal::AttackEnd, 0.1f, false);
	}
}

void ALPAnimal::AttackEnd()
{
	auto LPPlayerState = GetLPPlayerState();

	if (LPPlayerState && LPPlayerState->GetCharacterLevel() >= 5)
	{
		AttackRadius = 100.0f * LPPlayerState->GetScale();
		AttackRange = 200.0f * LPPlayerState->GetScale();;
	}
	else
	{
		AttackRadius = 100.0f;
		AttackRange = 200.0f;
	}

	AnimalAnim->SetIsAttacking(false);
}

void ALPAnimal::AttackCheck()
{
	auto LPPlayerState = Cast<ALPPlayerState>(GetPlayerState());

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			LPLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			auto LPAnimal = Cast<ALPAnimal>(HitResult.Actor);

			if (LPAnimal && LPAnimal->ActorHasTag(TEXT("Enemy")) && LPAnimal->ActorHasTag(TEXT("Player")))
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(LPPlayerState->GetAttack(ELPSkills::Others), DamageEvent, GetController(), this);
			}
		}
	}
}

void ALPAnimal::Attack(ELPSkills Skill)
{
	if (AnimalAnim->GetIsDead()) return;

	AttackStart();
	AttackCheck();
}
