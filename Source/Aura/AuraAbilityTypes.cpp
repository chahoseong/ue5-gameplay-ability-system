#include "AuraAbilityTypes.h"


UScriptStruct* FAuraGameplayEffectContext::GetScriptStruct() const
{
	return FGameplayEffectContext::StaticStruct();
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return true;
}

bool FAuraGameplayEffectContext::IsCriticalHit() const
{
	return bIsCriticalHit;
}

bool FAuraGameplayEffectContext::IsBlockedHit() const
{
	return bIsBlockedHit;
}

void FAuraGameplayEffectContext::SetCriticalHit(bool bInIsCriticalHit)
{
	bIsCriticalHit = bInIsCriticalHit;
}

void FAuraGameplayEffectContext::SetBlockedHit(bool bInIsBlockedHit)
{
	bIsBlockedHit = bInIsBlockedHit;
}
