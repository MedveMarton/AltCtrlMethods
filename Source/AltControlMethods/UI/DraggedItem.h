// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggedItem.generated.h"

class UImage;
class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UDraggedItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category="Dragged Icon", meta=(BindWidget))
	UBorder* Border;
	UPROPERTY(VisibleAnywhere, Category="Dragged Icon", meta=(BindWidget))
	UImage* Icon;
	UPROPERTY(VisibleAnywhere, Category="Dragged Icon", meta=(BindWidget))
	UTextBlock* Quantity;

	
};
