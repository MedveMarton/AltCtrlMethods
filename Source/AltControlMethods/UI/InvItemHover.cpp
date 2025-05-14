// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/UI/InvItemHover.h"
#include "InvSlot.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "Components/TextBlock.h"

void UInvItemHover::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemHovered = InvSlotHovered->GetItemRef();

	switch (ItemHovered->ItemType) {
	case EItemType::General:
		ItemType->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Type", "Általános"), 1));
		ItemDamage->SetVisibility(ESlateVisibility::Collapsed);
		ItemSpeed->SetVisibility(ESlateVisibility::Collapsed);
		ItemRange->SetVisibility(ESlateVisibility::Collapsed);
		ItemHealing->SetVisibility(ESlateVisibility::Collapsed);
		MagSize->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::RangedWeapon:
		ItemType->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Type", "Lőfegyver"), 1));
		ItemHealing->SetVisibility(ESlateVisibility::Collapsed);
		MagSize->SetText(FText::Format(NSLOCTEXT("InvItemHover", "MagSize", "Tárkapacitás: {0}"), ItemHovered->ItemStats.MagSize));
		break;
	case EItemType::MeleeWeapon:
		ItemType->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Type", "Közelharci fegyver"), 1));
		ItemHealing->SetVisibility(ESlateVisibility::Collapsed);
		MagSize->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Type", "Fogyasztható"), 1));
		ItemSpeed->SetVisibility(ESlateVisibility::Collapsed);
		ItemRange->SetVisibility(ESlateVisibility::Collapsed);
		MagSize->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Ammunition:
		ItemType->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Type", "Lőszer"), 1));
		ItemHealing->SetVisibility(ESlateVisibility::Collapsed);
		MagSize->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default: ;
	}

	ItemName->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Name", "{0}"),  ItemHovered->ItemText.Name));
	ItemDesc->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Desc", "{0}"),  ItemHovered->ItemText.Desc));
	ItemUsage->SetText(FText::Format(NSLOCTEXT("InvItemHover", "InvUseText", "Használat: {0}"),  ItemHovered->ItemText.InvUseText));
	ItemDamage->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Damage", "Sebzés: {0}"), ItemHovered->ItemStats.Damage));
	ItemSpeed->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Speed", "Sebesség: {0}"), ItemHovered->ItemStats.Speed));
	ItemRange->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Range", "Hatótáv: {0}"), ItemHovered->ItemStats.Range));
	ItemHealing->SetText(FText::Format(NSLOCTEXT("InvItemHover", "Healing", "Gyógyítás: {0}"), ItemHovered->ItemStats.Healing));
	ItemStackWgt->SetText(FText::Format(NSLOCTEXT("InvItemHover", "StackWeight", "Tömeg: {0}"), ItemHovered->GetItemStackWeight()));

	if (ItemHovered->InvItemStats.bIsStackable)
	{
		ItemStackSize->SetText(FText::Format(NSLOCTEXT("InvItemHover", "StackSize", "Max Db: {0}"), ItemHovered->InvItemStats.StackSize));
	}
	else
	{
		ItemStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}