// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
	// Disable collision, instead we use SphereComp to handle interaction queries.
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	
	USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
	
	// Checking if the player is at full health.
	if(ensure(AttributeComponent) && !AttributeComponent->IsFullHealth())
	{
		// This will only happen if the player is not at full health.
		if(AttributeComponent->ApplyHealthChange(this, AttributeComponent->GetHealthMax()))
		{
			HideAndCooldownPowerup();
			PlayerState->SubtractScore(1.0f);
		}
	}
}