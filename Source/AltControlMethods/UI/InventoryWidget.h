// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UWrapBox;
class UInvSlot;
class UInventoryComponent;
class AAltControlMethodsCharacter;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void RedrawInv();

	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryWidget;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WgtInfo;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapInfo;

	UPROPERTY()
	AAltControlMethodsCharacter* User;

	UPROPERTY()
	UInventoryComponent* InvRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInvSlot> InvSlotClass;

protected:
	void SetInfoText() const;

	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
