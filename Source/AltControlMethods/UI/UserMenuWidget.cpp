// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/UI/UserMenuWidget.h"

#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"

#include "AltControlMethods/UI/ItemDragDropOp.h"

void UUserMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUserMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AltCtrlMetChar = Cast<AAltControlMethodsCharacter>(GetOwningPlayerPawn());
}

bool UUserMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOp* ItemDragDropOp = Cast<UItemDragDropOp>(InOperation);

	if (AltCtrlMetChar && ItemDragDropOp->SourceItem != nullptr)
	{
		AltCtrlMetChar->DropItem(ItemDragDropOp->SourceItem, ItemDragDropOp->SourceItem->Quantity);
		return true;
	}
	return false;
}
