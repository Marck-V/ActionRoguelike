// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
	MaxHealth = 100;
}

bool USAttributeComponent::isAlive() const
{
	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}



