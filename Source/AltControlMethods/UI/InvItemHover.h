// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvItemHover.generated.h"

class UTextBlock;
class UInvSlot;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UInvItemHover : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(VisibleAnywhere)
	UInvSlot* InvSlotHovered;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDesc;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemUsage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemType;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDamage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemSpeed;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemRange;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MagSize;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemHealing;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemStackSize;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemStackWgt;

protected:

	virtual void NativeConstruct() override;
};
