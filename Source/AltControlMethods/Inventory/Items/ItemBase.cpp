// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "AltControlMethods/Inventory/InventoryComponent.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/RangedWeaponActor.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->ActorClass = this->ActorClass;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemStats = this->ItemStats;
	ItemCopy->ItemText = this->ItemText;
	ItemCopy->InvItemStats = this->InvItemStats;
	ItemCopy->ItemAssets = this->ItemAssets;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuant)
{
	if (NewQuant != Quantity)
	{
		Quantity = FMath::Clamp(NewQuant, 0, InvItemStats.bIsStackable ? InvItemStats.StackSize : 1);

		if (Inv != nullptr)
		{
			if (Quantity <= 0)
			{
				Inv->RemoveItem(this);
			}
		}
	}
}

void UItemBase::Use(AAltControlMethodsCharacter* Character)
{
	switch (this->ItemType) {
	case EItemType::General:
		UE_LOG(LogTemp, Warning, TEXT("General Use Called"));
		break;
	case EItemType::RangedWeapon:
		UE_LOG(LogTemp, Warning, TEXT("RangedWeapon Use Called"));

		if (this->ActorClass != nullptr)
		{
			if (this->Inv == Character->GetInv())
			{
				//Toggle gun attachment as the item is still in inv
				OnUse.Broadcast(Character);
				UE_LOG(LogTemp, Warning, TEXT("Inv Same"));
				Character->EquipRangedItem(this);
			}
		}
		
		break;
	case EItemType::MeleeWeapon:
		UE_LOG(LogTemp, Warning, TEXT("MeleeWeapon Use Called"));
		OnUse.Broadcast(Character);
		if (this->Inv == Character->GetInv())
		{
			//Toggle gun attachment as the item is still in inv
			OnUse.Broadcast(Character);
			UE_LOG(LogTemp, Warning, TEXT("Inv Same"));
			Character->EquipRangedItem(this);
		}
		break;
	case EItemType::Consumable:
		UE_LOG(LogTemp, Warning, TEXT("Consumable Use Called"));
		break;
	case EItemType::Ammunition:
		UE_LOG(LogTemp, Warning, TEXT("Ammunition Use Called"));
		break;
	default: ;
	}
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}