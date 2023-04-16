// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "GameFramework/Actor.h"
#include "LPFloatingLog.generated.h"

UCLASS()
class LIONPROJECT_API ALPFloatingLog : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALPFloatingLog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Actor)
	UStaticMeshComponent* Mesh;
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	FVector MovementDirection;

};
