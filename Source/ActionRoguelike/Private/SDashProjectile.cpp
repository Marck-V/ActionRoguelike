// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

void ASDashProjectile::Explode_Implementation()
{
	//Super::Explode_Implementation();

	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	EffectComponent->Deactivate();

	MovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportToInstigator, TeleportDelay);
	
}

void ASDashProjectile::TeleportToInstigator()
{
	// Setting a variable to get the instigator so then we know what actor to teleport.
	AActor* ActorToTeleport = GetInstigator();
	if(ensure(ActorToTeleport))
	{
		// Setting the location of the actor to the location of the projectile and keeping actor rotation so it is not a jarring teleport.
		ActorToTeleport->TeleportTo(GetActorLocation(),ActorToTeleport->GetActorRotation(),false,false);
	}
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Get World timer manager is getting the timer instance from each actor in the world and in this case we want to handle to the delayed detonate timer.
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
	
}

ASDashProjectile::ASDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComponent->InitialSpeed = 6000.0f;
}

