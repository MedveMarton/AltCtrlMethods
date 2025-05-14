// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserMenuWidget.generated.h"

class AAltControlMethodsCharacter;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UUserMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;

public:

	UPROPERTY()
	AAltControlMethodsCharacter* AltCtrlMetChar;
};
