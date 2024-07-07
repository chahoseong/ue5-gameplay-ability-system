// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEnemy.h"
#include "DrawDebugHelpers.h"

AAuraEnemy::AAuraEnemy()
{

}

void AAuraEnemy::HighlightActor()
{
	bHighlighting = true;
}

void AAuraEnemy::UnhighlightActor()
{
	bHighlighting = false;
}
