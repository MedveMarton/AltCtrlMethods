// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InterWidget.generated.h"

class UProgressBar;
class UTextBlock;
struct FInteractableData;
class AAltControlMethodsCharacter;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UInterWidget : public UUserWidget
{
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Inter Widget | User Ref")
	AAltControlMethodsCharacter* UserRef;

	void UpdateWidget(const FInteractableData* InteractableData) const;
	
protected:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inter Widget | Interable Data")
	UTextBlock* NameTxt;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inter Widget | Interable Data")
	UTextBlock* ActionTxt;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inter Widget | Interable Data")
	UTextBlock* QuantityTxt;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inter Widget | Interable Data")
	UTextBlock* KeyPressTxt;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inter Widget | Interable Data")
	UProgressBar* InterProgBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inter Widget | Interable Data")
	float CurrentInterDur;

	UFUNCTION(Category = "Inter Widget | Interable Data")
	float UpdateInterProg();

	
		
};
