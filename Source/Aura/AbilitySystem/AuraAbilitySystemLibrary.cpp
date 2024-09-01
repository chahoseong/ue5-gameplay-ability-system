// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystemComponent.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = PlayerController->GetHUD<AAuraHUD>())
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = PlayerController->GetHUD<AAuraHUD>())
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return AuraHUD->GetAttributeWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		AActor* AvatarActor = ASC->GetAvatarActor();
		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		/* 
		 * Primary Attributes 
		 */

		FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
		PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);

		const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);


		/* 
		 * Secondary Attributes
		 */

		FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
		SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

		const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

		/* 
		 * Vital Attributes
		 */

		FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
		VitalAttributesContextHandle.AddSourceObject(AvatarActor);

		const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	}
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return AuraGameMode->CharacterClassInfo;
	}

	return nullptr;
}
