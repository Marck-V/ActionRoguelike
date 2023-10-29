// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 		PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
		AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
	float Delta)
{
	
	// If the delta is less than zero and the new health is less than or equal to zero, then we are dead
	if(NewHealth <= 0.0f && Delta < 0.0f)
	{
		// Add code here to handle death
		AAIController* AIC = Cast<AAIController>(GetController());
	}
	// If the new health is less than or equal to 50, then we are low health
	if(NewHealth <= 50)
	{
		AAIController* AIC = Cast<AAIController>(GetController());
		// If the AI controller is valid, then we can set the blackboard value
		if(AIC)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Low Health");
		}
	}
}

void ASAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", SeenPawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Seen Pawn", nullptr, FColor::Yellow, 3.0f, true);
	}
}




