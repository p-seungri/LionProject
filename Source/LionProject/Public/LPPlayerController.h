// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LionProject.h"
#include "GameFramework/PlayerController.h"
#include "LPPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class LIONPROJECT_API ALPPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALPPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	FTimerHandle HUDTimer;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UUserWidget* HUDWidget;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UUserWidget* GameOverWidget;

public:
	virtual void Tick(float DeltaTime) override;

	void NPCKill(class ALPAnimal* KilledNPC) const;

	void SetGameOverWidget();

private:


};
