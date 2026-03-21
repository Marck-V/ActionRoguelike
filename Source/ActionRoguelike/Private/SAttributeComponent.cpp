// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultipler(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Multiplier for Attribute Component"), ECVF_Cheat);

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

	// Setting the max rage to 100 then setting the rage to the max rage.
	Rage = 0.0f;
	MaxRage = 100;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	const float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);

	float ActualDelta = Rage - OldRage;

	// Will be zero delta if we already at max or min
	if (!FMath::IsNearlyZero(ActualDelta))
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
		return true;
	}

	return false;
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
	// If the owner of the attribute component cannot be damaged and the delta is less than 0, then we return false.
	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	// If the delta is less than 0, then we multiply the delta by the damage multiplier.
	if(Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultipler.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;

	// Clamping the health to ensure it is between the minimum and maximum when the health changes.
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	// Dead
	if(ActualDelta < 0.0f && !IsAlive())
	{
		// If the health change is negative, then the actor has taken damage.
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
		
	}
	
	return ActualDelta != 0.0f;
}



