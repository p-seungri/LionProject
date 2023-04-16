// Fill out your copyright notice in the Description page of Project Settings.


#include "LPZebra.h"

ALPZebra::ALPZebra()
{
	GetMesh()->SetRelativeLocation(FVector(0.0f, -15.0f, -100.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UPhysicsAsset> PA_ZEBRA(TEXT("/Game/AfricanAnimalsPack/Zebra/Meshes/SK_Zebra_PhysicsAsset.SK_Zebra_PhysicsAsset"));

	if (PA_ZEBRA.Succeeded())
	{
		GetMesh()->SetPhysicsAsset(PA_ZEBRA.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ZEBRA(TEXT("/Game/AfricanAnimalsPack/Zebra/Meshes/SK_Zebra.SK_Zebra"));

	if (SK_ZEBRA.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ZEBRA.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZEBRA_ANIM(TEXT("/Game/AfricanAnimalsPack/Zebra/Animations/LPZebraAnimBP.LPZebraAnimBP_C"));

	if (ZEBRA_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ZEBRA_ANIM.Class);
	}

	AnimalType = EAnimalType::Zebra;
}
