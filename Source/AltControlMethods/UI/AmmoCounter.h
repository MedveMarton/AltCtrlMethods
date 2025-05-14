// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AmmoCounter.generated.h"

/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UAmmoCounter : public UUserWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY()
	AAltControlMethodsCharacter* User;

	UPROPERTY()
	UInventoryComponent* InvRef;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AmmoCounterText;

	virtual void NativeOnInitialized() override;

	void UpdateAmmoCounter() const;
	void SetAmmoText(int AmmoMag, int AmmoTot) const;
};
