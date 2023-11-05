// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ASMagicProjectile::ASMagicProjectile()
{
	SphereComponent->SetSphereRadius(20.0f);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	DamageAmount = 20.0f;
}

float ASMagicProjectile::GetDamageAmount()
{
	return DamageAmount;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is the instigator
	if(OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		
		// If it has the parry tag then reverse the velocity and set the instigator to the other pawn.
		if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComponent->Velocity = -MovementComponent->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		// Apply Damage & Impulse
		if(USGameplayFunctionLibrary::ApplyDirectionDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
		}
	}
}



void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

