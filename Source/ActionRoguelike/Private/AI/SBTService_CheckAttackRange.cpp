// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI pawn and target actor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AActor* MyActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(MyActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if(ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if(ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(MyActor->GetActorLocation(), AIPawn->GetActorLocation());
					
					bool bInAttackRange = DistanceTo < 2000.f;
					
					bool bHasLOS = false;
					
					if(bInAttackRange)
					{
						bHasLOS = MyController->LineOfSightTo(MyActor);
					}
					
					
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bInAttackRange && bHasLOS));
					
					
				}
			}
		}
	}
}
