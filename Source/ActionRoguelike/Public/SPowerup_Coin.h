// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Coin.generated.h"

/**
 * 
 */
class UStaticMeshComponent;
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Coin : public ASPowerupActor
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float CreditsAmount;
	void Interact_Implementation(APawn* InstigatorPawn) override;

	ASPowerup_Coin();
	
};
