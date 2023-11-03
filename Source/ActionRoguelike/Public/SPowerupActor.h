// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

class ASPlayerState;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:

	ASPlayerState* PlayerState;
	
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float RespawnTime;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComponent;
	
public:	

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
