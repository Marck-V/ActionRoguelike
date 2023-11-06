// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

int32 ASPlayerState::GetCredits() const
{
	return Credit;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	if(!ensure(Delta > 0.0f))
	{
		return;
	}

	Credit += Delta;

	OnCreditsChanged.Broadcast(this,Credit, Delta);
	
}

bool ASPlayerState::SubtractCredits(int32 Delta)
{
	// Avoid user-error of adding or subtracting a negative amount or zero
	if(!ensure(Delta > 0.0f))
	{
		return false;
	}
	if(Credit < Delta)
	{
		// Happens when you do not have enough credits
		return false;
	}

	Credit -= Delta;

	OnCreditsChanged.Broadcast(this,Credit, -Delta);

	return true;
}

