// Fill out your copyright notice in the Description page of Project Settings.


#include "LPCrocodile.h"
#include <LPPlayerState.h>


ALPCrocodile::ALPCrocodile()
{
	GetMesh()->SetRelativeLocation(FVector(0.0f, -15.0f, -100.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UPhysicsAsset> PA_CROCODILE(TEXT("/Game/AfricanAnimalsPack/Crocodile/Meshes/SK_Crocodile_PhysicsAsset.SK_Crocodile_PhysicsAsset"));

	if (PA_CROCODILE.Succeeded())
	{
		GetMesh()->SetPhysicsAsset(PA_CROCODILE.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CROCODILE(TEXT("/Game/AfricanAnimalsPack/Crocodile/Meshes/SK_Crocodile.SK_Crocodile"));

	if (SK_CROCODILE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CROCODILE.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CROCODILE_ANIM(TEXT("/Game/AfricanAnimalsPack/Crocodile/Animations/LPCrocodileAnimBP.LPCrocodileAnimBP_C"));

	if (CROCODILE_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CROCODILE_ANIM.Class);
	}

	AnimalType = EAnimalType::Crocodile;



}

void ALPCrocodile::BeginPlay()
{
	Super::BeginPlay();

	
}

void ALPCrocodile::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}
