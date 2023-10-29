// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

#include "Components/SphereComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Powerup");
	RootComponent = SphereComponent;

	RespawnTime = 10.0f;

}

void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	// Setting the collision to false 
	SetActorEnableCollision(false);
	// Setting the visibility to false while it is on cooldown.
	RootComponent->SetVisibility(false, true);
	// Setting the visibility to false while it is on cooldown.
	FTimerHandle TimerHandle_Respawn;
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	RootComponent->SetVisibility(bNewIsActive, true);
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Logic is derived in base class
}



