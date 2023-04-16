// Fill out your copyright notice in the Description page of Project Settings.


#include "LPFloatingLog.h"

// Sets default values
ALPFloatingLog::ALPFloatingLog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_LOG(TEXT("/Game/RuralAustralia/StaticMeshes/Vegetation/Log_S_02/SM_Log_S_02.SM_Log_S_02"));

	if (SM_LOG.Succeeded())
	{
		Mesh->SetStaticMesh(SM_LOG.Object);
	}

	Mesh->SetCollisionProfileName(TEXT("FloatingPreset"));

	CurrentSpeed = 600.0f;
	MovementDirection = FVector(0.0f, -1.0f, 0.0f);
}

// Called when the game starts or when spawned
void ALPFloatingLog::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(1.5f));
}

// Called every frame
void ALPFloatingLog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(MovementDirection * (CurrentSpeed * DeltaTime));
}

