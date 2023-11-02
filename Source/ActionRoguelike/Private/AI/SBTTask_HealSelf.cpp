// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Setting the owner of the behavior tree to the AI controller.
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	// If the pawn is null, return failed.
	if(MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	// Getting the attribute component from the pawn.
	USAttributeComponent* AtttributeComp = USAttributeComponent::GetAttributes(MyPawn);
	
	// If the attribute component is true, apply the health change to the pawn.
	if(ensure(AtttributeComp))
	{
		AtttributeComp->ApplyHealthChange(MyPawn, AtttributeComp->GetHealthMax());
	}
	// Return succeeded.
	return EBTNodeResult::Succeeded;
}
