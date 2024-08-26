// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	const bool bInLocallyControlled = Ability->IsLocallyControlled();

	if (bInLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
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

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	const FHitResult* HitResult = DataHandle.Get(0)->GetHitResult();

	UE_LOG(LogTemp, Warning, TEXT("[Serve] Hit Location: (%f, %f, %f)"), HitResult->Location.X, HitResult->Location.Y, HitResult->Location.Z);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
