// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UsageWidget.generated.h"

class UInvSlot;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UUsageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	AAltControlMethodsCharacter* UserChar;

	UPROPERTY(VisibleAnywhere)
	UInvSlot* InvSlotUsed;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemUsage;
	
protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
