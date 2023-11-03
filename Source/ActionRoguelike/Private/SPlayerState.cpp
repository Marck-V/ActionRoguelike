// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

float ASPlayerState::AddScore(float ScoreDelta)
{
	Credit += ScoreDelta;
	return Credit;
}

void ASPlayerState::SubtractScore(float ScoreDelta)
{
	Credit -= ScoreDelta;
}
