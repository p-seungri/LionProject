// Fill out your copyright notice in the Description page of Project Settings.


#include "LPPlayerController.h"
#include <LPPlayerState.h>
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <LPAnimal.h>

ALPPlayerController::ALPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD_C(TEXT("/Game/LPUI/UI_HUD.UI_HUD_C"));

	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_GAMEOVER_C(TEXT("/Game/LPUI/UI_GameOver.UI_GameOver_C"));

	if (UI_GAMEOVER_C.Succeeded())
	{
		GameOverWidgetClass = UI_GAMEOVER_C.Class;
	}

	bShowMouseCursor = true;
}

void ALPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameInputOnly;
	SetInputMode(GameInputOnly);


	HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	GameOverWidget = CreateWidget<UUserWidget>(this, GameOverWidgetClass);

	if (HUDWidget != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(HUDTimer, FTimerDelegate::CreateLambda([this]() -> void
			{
				HUDWidget->AddToViewport();
			}), 5.0f, false);
	}
	
}

void ALPPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ALPPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALPPlayerController::NPCKill(ALPAnimal* KilledNPC) const
{
	auto LPPlayerState = Cast<ALPPlayerState>(PlayerState);

	if (LPPlayerState != nullptr)
	{
		auto LPKilledPlayerState = Cast<ALPPlayerState>(KilledNPC->GetPlayerState());

		if (LPKilledPlayerState != nullptr)
		{
			LPPlayerState->AddExp(LPKilledPlayerState->GetDropExp());
		}
	}

}

void ALPPlayerController::SetGameOverWidget()
{
	if (GameOverWidget != nullptr && !GameOverWidget->IsVisible())
	{
		HUDWidget->RemoveFromParent();
		GameOverWidget->AddToViewport();
		bShowMouseCursor = true;
	}
}

