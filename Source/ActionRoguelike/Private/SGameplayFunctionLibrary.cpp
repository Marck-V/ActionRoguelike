// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if(ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// tip: When we want the direction we start with the target and subtract it from the origin.
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			HitComp->AddImpulseAtLocation(Direction * 30000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
