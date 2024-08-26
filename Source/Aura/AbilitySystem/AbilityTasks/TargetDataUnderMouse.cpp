// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"
#include "Kismet/GameplayStatics.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	APlayerController* Player = ActorInfo->PlayerController.Get();
	FHitResult CursorHit;
	Player->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	ValidData.Broadcast(CursorHit.Location);
}
