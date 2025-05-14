// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/UI/UsageWidget.h"
#include "InvSlot.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "Components/TextBlock.h"

void UUsageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemUsed = InvSlotUsed->GetItemRef();

	UserChar = Cast<AAltControlMethodsCharacter>(GetOwningPlayerPawn());


	if (UserChar->MainWepEquipped && UserChar->MainWepEquipped == ItemUsed->ActorClassRef)
	{
		ItemUsage->SetText(FText::Format(NSLOCTEXT("InvItemUsed", "InvUseText", "{0}"),  ItemUsed->ItemText.InvAltUseText));
	} else
	{
		ItemUsage->SetText(FText::Format(NSLOCTEXT("InvItemUsed", "InvUseText", "{0}"), ItemUsed->ItemText.InvUseText));
	}
	
}

void UUsageWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	RemoveFromParent();
}

FReply UUsageWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Call Usage"));
		UItemBase* ItemUsed = InvSlotUsed->GetItemRef();
		ItemUsed->Use(UserChar);
		
		return Reply.Handled();
	}
	return Reply.Unhandled();
}
