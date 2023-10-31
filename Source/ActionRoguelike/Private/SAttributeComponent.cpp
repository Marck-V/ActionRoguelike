// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

// Sets default values
USAttributeComponent::USAttributeComponent()
{
	// Setting the max health to 100 then setting the health to the max health.
	MaxHealth = 100;
	Health = MaxHealth;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

float USAttributeComponent::GetHealthMax() const
{
	return MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	float OldHealth = Health;

	// Clamping the health to ensure it is between the minimum and maximum when the health changes.
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	return ActualDelta != 0.0f;
}



