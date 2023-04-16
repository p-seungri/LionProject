// Fill out your copyright notice in the Description page of Project Settings.


#include "LPLioness.h"

ALPLioness::ALPLioness()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_LIONESS(TEXT("/Game/AfricanAnimalsPack/LionAndLioness/Meshes/SK_Lioness_LOD0.SK_Lioness_LOD0"));

	if (SK_LIONESS.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_LIONESS.Object);
	}
}

void ALPLioness::BeginPlay()
{
	Super::BeginPlay();

}
