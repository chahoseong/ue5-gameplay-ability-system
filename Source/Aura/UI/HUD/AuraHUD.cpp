// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

void AAuraHUD::InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, pleases fill out BP_AuraHUD"));
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Controller Class uninitialized, pleases fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams Params(InPlayerController, InPlayerState, InAbilitySystemComponent, InAttributeSet);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(Params);

	OverlayWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& InParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(InParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeWidgetController(const FWidgetControllerParams& InParams)
{
	if (!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(InParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}
