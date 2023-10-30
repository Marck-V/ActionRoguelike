// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class UUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	
	USWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	void SetTargetActor(AActor* NewTarget);
	
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USAttributeComponent* AttributeComponent;
	
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
};
