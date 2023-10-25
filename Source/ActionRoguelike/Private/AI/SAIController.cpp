// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr! Please assign your behavior tree in your AI controller for %s"), *GetName()))
	{
		RunBehaviorTree(BehaviorTree);
	}

	
	// Setting the MyPawn variable to the player pawn
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// If MyPawn is not null then set the MoveToLocation to the player pawn's location.
// 	if(MyPawn)
// 	{
// 		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
//
// 		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
// 	}
}
