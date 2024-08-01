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

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
    const bool bDebugDraw = CVarDebugInteraction.GetValueOnGameThread();

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    AActor* MyOwner = GetOwner();

    FVector EyeLocation;
    FRotator EyeRotation;
    MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

    FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

    TArray<FHitResult> Hits;

    FCollisionShape Shape;
    Shape.SetSphere(TraceRadius);

    bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
    FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

    // Clear the focused actor before we start the loop.
    FocusedActor = nullptr;

    // For loop to iterate through all the hits.
    for (FHitResult Hit : Hits)
    {
        if (bDebugDraw)
        {
            DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
        	// Print Message to Screen
        	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit"));
        }
        // If the hit actor implements the SGameplayInterface, then we call the Interact function.
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            if (HitActor->Implements<USGameplayInterface>())
            {
                FocusedActor = HitActor;
                break;
            }
        }
    }

    // If the focused actor is valid, then we create a widget instance of the default widget class.
    if (FocusedActor)
    {
        if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
        {
            DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
        }
        // If the default widget instance is valid, then we set the attached actor to the focused actor.
        if (DefaultWidgetInstance)
        {
            DefaultWidgetInstance->AttachedActor = FocusedActor;
            if (!DefaultWidgetInstance->IsInViewport())
            {
                DefaultWidgetInstance->AddToViewport();
            }
        }
    }
    else
    {
        if (DefaultWidgetInstance)
        {
            DefaultWidgetInstance->RemoveFromParent();
        }
    }

    // if debug draw is true, then we draw a debug line at the impact point.
    if (bDebugDraw)
    {
        DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
    }
}

void USInteractionComponent::PrimaryInteract()
{
	if(FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No Focused Actor"));
		return;
	}
	// We cast the owner to a pawn so that we can pass it to the Interact function.
	APawn* MyPawn = CastChecked<APawn>(GetOwner());
	// We call the Interact function on the hit actor.
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
