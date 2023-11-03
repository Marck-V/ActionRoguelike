// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "PlayerState")
	int Credit;
	
	UFUNCTION()
	float AddScore(float ScoreDelta);

	UFUNCTION()
	void SubtractScore(float ScoreDelta);
};
