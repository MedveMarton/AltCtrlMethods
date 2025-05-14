// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoCounter.h"
#include "GeneralCharHUD.h"
#include "GameFramework/HUD.h"
#include "AltCtrlMetHUD.generated.h"

struct FInteractableData;
class UUserMenuWidget;
class UInterWidget;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API AAltCtrlMetHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UInterWidget> InterWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserMenuWidget> UserMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UGeneralCharHUD> GenCharHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UAmmoCounter> AmmoCounterClass;
	
	bool IsMenuVisible;
	
	AAltCtrlMetHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInterWidget() const;
	void HideInterWidget() const;
	void UpdateInterWidget(const FInteractableData* InteractableData) const;

	void DisplayAmmoCounter() const;
	void UpdateAmmoCounter(int AmmoInMag, int AmmoTotal) const;
	void HideAmmoCounter() const;
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	UUserMenuWidget* UserMenuWidget;

	UPROPERTY()
	UInterWidget* InterWidget;

	UPROPERTY()
	UGeneralCharHUD* GeneralCharHUDWidget;

	UPROPERTY()
	UAmmoCounter* AmmoCounterWidget;
;
};
