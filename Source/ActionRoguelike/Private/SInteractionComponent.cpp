// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugInteraction(TEXT("su.DebugInteraction"), false, TEXT("Debug Interaction for Interaction Component"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* MyOwner = GetOwner();

	FVector Start;
	

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams, Shape);

	TArray<FHitResult> Hits;
	float Radius = 30;
	FCollisionShape Shape;
	Shape.SetSphere(30.0f);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// For loop to iterate through all the hits.
	for(FHitResult Hit : Hits)
	{
		if(bDebugDraw)
        	{
			DrawDebugSphere(GetWorld(),Hit.ImpactPoint, Radius,32, LineColor, false, 2.0f);
        	}
		// If the hit actor implements the SGameplayInterface, then we call the Interact function.
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				// We cast the owner to a pawn so that we can pass it to the Interact function.
				APawn* MyPawn = Cast<APawn>(MyOwner);
				// We call the Interact function on the hit actor.
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		// if debug draw is true, then we draw a debug sphere at the impact point.
		if(bDebugDraw)
		{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
		}
	}
	
}
