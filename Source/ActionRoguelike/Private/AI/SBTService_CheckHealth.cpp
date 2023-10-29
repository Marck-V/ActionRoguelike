// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
		if(ensure(AIPawn))
		{
			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
			if(ensure(AttributeComp))
			{
				bool bIsHealthLow = AttributeComp->GetHealth() <= (AttributeComp->GetHealthMax() * 0.5f);
				BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsHealthLow);
			}
		}
	}
}
