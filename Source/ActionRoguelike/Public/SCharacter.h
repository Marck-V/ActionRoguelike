// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USAttributeComponent;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	
	UPROPERTY(EditAnywhere, Category= "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UParticleSystem* CastingEffect;
	
	UPROPERTY(EditAnywhere, Category= "Attack")
	TSubclassOf<AActor> BlackholeProjectile;
	
	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* AttackAnim2;
	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditAnywhere, Category= "Attack")
	TSubclassOf<AActor> DashProjectile;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* DashAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_Dash;
	FTimerHandle TimerHandle_BlackholeAttack;
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USAttributeComponent* AttributeComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	virtual void Jump() override;
	void PrimaryInteract();
	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack();
	void Dash();
	void Dash_TimeElapsed();
	void StartAttackEffects();
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	float AttackAnimDelay;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);

};
