// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values
USAttributeComponent::USAttributeComponent()
{
	// Setting the max health to 100 then setting the health to the max health.
	MaxHealth = 100;
	Health = MaxHealth;
}

bool USAttributeComponent::isFullHealth() const
{
	return Health == MaxHealth;
}

float USAttributeComponent::GetHealthMax() const
{
	return MaxHealth;
}

bool USAttributeComponent::isAlive() const
{
	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
	return ActualDelta != 0.0f;
}



