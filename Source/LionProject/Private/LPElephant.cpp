// Fill out your copyright notice in the Description page of Project Settings.


#include "LPElephant.h"

ALPElephant::ALPElephant()
{
	GetMesh()->SetRelativeLocation(FVector(0.0f, -15.0f, -100.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SK_ElephantTusksSmall = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TUSKSMALL"));
	SK_ElephantTusksMedium = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TUSKMEDIUM"));
	SK_ElephantTusksBig = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TUSKBIG"));


	static ConstructorHelpers::FObjectFinder<UPhysicsAsset> PA_ELEPHANT(TEXT("/Game/AfricanAnimalsPack/Elephant/Meshes/SK_Elephant_PhysicsAsset.SK_Elephant_PhysicsAsset"));

	if (PA_ELEPHANT.Succeeded())
	{
		GetMesh()->SetPhysicsAsset(PA_ELEPHANT.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELEPHANT(TEXT("/Game/AfricanAnimalsPack/Elephant/Meshes/SK_Elephant.SK_Elephant"));

	if (SK_ELEPHANT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ELEPHANT.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELEPHANTTUSKSMALL(TEXT("/Game/AfricanAnimalsPack/Elephant/Meshes/SK_ElephantTusksSmall.SK_ElephantTusksSmall"));

	if (SK_ELEPHANTTUSKSMALL.Succeeded())
	{
		SK_ElephantTusksSmall->SetSkeletalMesh(SK_ELEPHANTTUSKSMALL.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELEPHANTTUSKMEDIUM(TEXT("/Game/AfricanAnimalsPack/Elephant/Meshes/SK_ElephantTusksMedium.SK_ElephantTusksMedium"));

	if (SK_ELEPHANTTUSKMEDIUM.Succeeded())
	{
		SK_ElephantTusksMedium->SetSkeletalMesh(SK_ELEPHANTTUSKMEDIUM.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELEPHANTTUSKBIG(TEXT("/Game/AfricanAnimalsPack/Elephant/Meshes/SK_ElephantTusksBig.SK_ElephantTusksBig"));

	if (SK_ELEPHANTTUSKBIG.Succeeded())
	{
		SK_ElephantTusksBig->SetSkeletalMesh(SK_ELEPHANTTUSKBIG.Object);
	}

	SK_ElephantTusksSmall->SetupAttachment(GetMesh());
	SK_ElephantTusksMedium->SetupAttachment(GetMesh());
	SK_ElephantTusksBig->SetupAttachment(GetMesh());

	SK_ElephantTusksSmall->SetMasterPoseComponent(GetMesh());
	SK_ElephantTusksMedium->SetMasterPoseComponent(GetMesh());
	SK_ElephantTusksBig->SetMasterPoseComponent(GetMesh());


	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ELEPHANT_ANIM(TEXT("/Game/AfricanAnimalsPack/Elephant/Animations/LPElephantAnimBP.LPElephantAnimBP_C"));

	if (ELEPHANT_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ELEPHANT_ANIM.Class);
	}

	AnimalType = EAnimalType::Elephant;
}

void ALPElephant::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SK_ElephantTusksSmall->SetVisibility(false);
	SK_ElephantTusksMedium->SetVisibility(false);
}
