// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/UI/InventoryWidget.h"

#include "InvSlot.h"
#include "ItemDragDropOp.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/InventoryComponent.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	User = Cast<AAltControlMethodsCharacter>(GetOwningPlayerPawn());
	if (User != nullptr)
	{
		InvRef = User->GetInv();
		if (InvRef != nullptr)
		{
			//Adding the redraw func to InvUpdate delegate (for broadcasting)
			InvRef->InvUpdate.AddUObject(this, &UInventoryWidget::RedrawInv);
			SetInfoText();
		}
	}
}

void UInventoryWidget::RedrawInv()
{
	if (InvRef != nullptr && InvSlotClass != nullptr)
	{
		InventoryWidget->ClearChildren();
		for (auto InvItem : InvRef->GetInvContent())
		{
			UInvSlot* ItemSlot = CreateWidget<UInvSlot>(this,InvSlotClass);
			ItemSlot->SetItemRef(InvItem);
			InventoryWidget->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}

void UInventoryWidget::SetInfoText() const
{
	WgtInfo->SetText(FText::Format(NSLOCTEXT("InventoryWidget", "WgtCap", "Tömeg: {0}/{1}"), InvRef->GetInvTotalWgt(), InvRef->GetWgtCap()));

	CapInfo->SetText(FText::Format(NSLOCTEXT("InventoryWidget", "SlotCap", "Helyek: {0}/{1}"), InvRef->GetInvContent().Num(), InvRef->GetSlotCap()));
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOp* ItemDragDropOp = Cast<UItemDragDropOp>(InOperation);

	if (ItemDragDropOp->SourceItem && InvRef != nullptr)
	{
		//true return megallitja a dropop-ot
		return true;
	}
	//drop op megtortenik
	return false;
}