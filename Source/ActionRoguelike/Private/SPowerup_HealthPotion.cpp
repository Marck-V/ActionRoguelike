// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"

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

	// Q: Can you explain this code?
	// A: We are casting the InstigatorPawn to USAttributeComponent, and if it is valid, we are calling the Heal function.
	USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if(ensure(AttributeComponent) && !AttributeComponent->isAlive())
	{
		if(AttributeComponent->ApplyHealthChange(AttributeComponent->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}