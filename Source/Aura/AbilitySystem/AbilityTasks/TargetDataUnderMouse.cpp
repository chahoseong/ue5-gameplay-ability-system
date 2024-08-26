// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"
#include "GameplayAbilityTargetTypes.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();

	const bool bInLocallyControlled = Ability->IsLocallyControlled();

	if (bInLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{

	}


}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	APlayerController* Player = ActorInfo->PlayerController.Get();
	FHitResult CursorHit;
	Player->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
