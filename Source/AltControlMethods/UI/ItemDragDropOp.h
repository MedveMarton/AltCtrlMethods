// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOp.generated.h"

class UInventoryComponent;
class UItemBase;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UItemDragDropOp : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UItemBase* SourceItem;
	UPROPERTY()
	UInventoryComponent* SourceInv;
	
};
