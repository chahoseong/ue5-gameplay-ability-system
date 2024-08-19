// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/* Begin Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	/* End Enemy Interface */

	virtual int32 GetPlayerLevel() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Level = 1;
};
