// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

// Console Variable to enable/disable bot spawning.
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable/Disable Bot Spawning"), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		//Removes the controller from the pawn.
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}


void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	// If the victim is a bot, then we want to give the player credit for the kill.
	ASAICharacter* Bot = Cast<ASAICharacter>(VictimActor);
	if(Bot)
	{
		ASCharacter* BotKiller = Cast<ASCharacter>(KillerActor);
		if(BotKiller)
		{
			ASPlayerState* KillerPlayerState = Cast<ASPlayerState>(BotKiller->GetPlayerState());
			if(KillerPlayerState)
			{
				
				KillerPlayerState->AddScore(1.0f);
				UE_LOG(LogTemp, Log, TEXT("Credits: %d"), KillerPlayerState->Credit);
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more enemies.
	// Actual amount of bots and wheter its allowed to spawn more is determined in the Spawn logic later.
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
	for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		// If the bot is alive, then we increment the counter.
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this); // @fixme: You could also pass in the player for kill credit.
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	// Checking if the console variable is set to false.
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning is disabled via console variable 'CVARSpawnBots'"));
		return;
	}
	
	// Code to check if we are allowed to spawn more bots.
	int32 NrOfAliveBots = 0;
	// Looping through all the bots in the world.
	for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		// If the bot is alive, then we increment the counter.
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			++NrOfAliveBots;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("There are %d bots alive"), NrOfAliveBots);
	
	float MaxBotCount = 10.0f;
	
	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if(NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At Maximum Bot Count, Skipping Bot Spawn"));
		return;
	}
	// Running the FindBotSpawn EQS that we created in the editor.
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
		
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Query Failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		// To Track the spawn locations of the bots.
		DrawDebugSphere(GetWorld(), Locations[0], 25.0f, 12, FColor::Red, false, 5.0f);
	}
}
