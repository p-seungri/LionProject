// Fill out your copyright notice in the Description page of Project Settings.


#include "LPHippo.h"

ALPHippo::ALPHippo()
{
	GetMesh()->SetRelativeLocation(FVector(0.0f, -15.0f, -100.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UPhysicsAsset> PA_HIPPO(TEXT("/Game/AfricanAnimalsPack/Hippopotamus/Meshes/SK_Hippopotamus_PhysicsAsset.SK_Hippopotamus_PhysicsAsset"));

	if (PA_HIPPO.Succeeded())
	{
		GetMesh()->SetPhysicsAsset(PA_HIPPO.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HIPPO(TEXT("/Game/AfricanAnimalsPack/Hippopotamus/Meshes/SK_Hippopotamus.SK_Hippopotamus"));

	if (SK_HIPPO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_HIPPO.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> HIPPO_ANIM(TEXT("/Game/AfricanAnimalsPack/Hippopotamus/Animations/LPHippoAnimBP.LPHippoAnimBP_C"));

	if (HIPPO_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HIPPO_ANIM.Class);
	}

	AnimalType = EAnimalType::Hippopotamus;
}
