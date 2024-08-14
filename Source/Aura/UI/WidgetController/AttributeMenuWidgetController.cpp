// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		const FGameplayAttribute Attribute = Pair.Value();
		Info.AttributeValue = Attribute.GetNumericValue(AuraAttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}
}
