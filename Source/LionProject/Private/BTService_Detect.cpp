// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include <LPAIController.h>
#include <LPAnimal.h>
#include <LPAnimalAnimInstance.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ALPAnimal>((OwnerComp.GetAIOwner()->GetPawn()));
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World) return;

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParms(NAME_None, false, ControllingPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParms
	);


	if (bResult)
	{
		
		for (auto const& OverlapResult : OverlapResults)
		{
			ALPAnimal* LPAnimal = Cast<ALPAnimal>(OverlapResult.GetActor());
			

			if (LPAnimal && (LPAnimal->GetController()->IsPlayerController() && LPAnimal->ActorHasTag(TEXT("Enemy"))))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ALPAIController::TargetKey, LPAnimal);
				ControllingPawn->AnimalAnim->SetIsSprint(true);
				ControllingPawn->GetCharacterMovement()->MaxWalkSpeed = 450.0f;
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, LPAnimal->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), LPAnimal->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}

		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ALPAIController::TargetKey, nullptr);

	auto LPAnimal = Cast<ALPAnimal>(ControllingPawn);

	if (nullptr == LPAnimal)
	{
		return;
	}

	ControllingPawn->AnimalAnim->SetIsSprint(false);
	ControllingPawn->GetCharacterMovement()->MaxWalkSpeed = 300.0f;

}
