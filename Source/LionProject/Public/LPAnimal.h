// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "GameFramework/Character.h"
#include "LPAnimal.generated.h"

UCLASS()
class LIONPROJECT_API ALPAnimal : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALPAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Attack(ELPSkills Skill);

	virtual void AttackInitialize();

	bool GetIsRest() const;
	bool GetIsAttack() const;
	bool GetIsPatrol() const;

	UFUNCTION(BlueprintCallable)
	void SetIsRest(bool Rest);

	UFUNCTION(BlueprintCallable)
	void SetIsAttack(bool Attack);

	UFUNCTION(BlueprintCallable)
	void SetIsPatrol(bool Patrol);

	UFUNCTION(BlueprintCallable)
	class ALPPlayerState* GetLPPlayerState();

	EAnimalType GetAnimalType() const;

	void OnDeath();

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UWidgetComponent* HPBarWidget;

	UPROPERTY()
	class ULPAnimalAnimInstance* AnimalAnim;

	UPROPERTY(BlueprintReadWrite)
	FVector DirectionToMove = FVector::ZeroVector;

	FTimerHandle HPBarTimer;
	FTimerHandle AttackEndTimer;
	FTimerHandle UpdateIsAttackTimer;
	FTimerHandle RestHandle;
	EAnimalType AnimalType;

	static float PlayerScale;


protected:
	virtual void MoveForward(float AxisValue);
	virtual void MoveRight(float AxisValue);
	void SprintStart();
	void SprintEnd();
	void RestToggle();
	void SetInvincible();

	
	void SetControlMode(EControlMode NewControlMode);


	EControlMode CurrentControlMode;
private:
	void AttackStart();
	void AttackEnd();
	void AttackCheck();


	bool RunawayStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsRest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsPatrol;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;
};
