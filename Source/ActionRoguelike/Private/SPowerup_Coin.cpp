// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Coin.h"

#include "SPlayerState.h"


ASPowerup_Coin::ASPowerup_Coin()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	// Disable collision, instead we use SphereComp to handle interaction queries.
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ASPowerup_Coin::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}

	PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());

	PlayerState->AddScore(1.0f);
	HideAndCooldownPowerup();
}