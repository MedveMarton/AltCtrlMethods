// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/ItemPickup.h"

#include "IDetailTreeNode.h"
#include "ItemBase.h"
#include "AltControlMethods/Inventory/InventoryComponent.h"

AItemPickup::AItemPickup()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	ItemPickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemPickupMesh");
	ItemPickupMesh->SetSimulatePhysics(true);
	SetRootComponent(ItemPickupMesh);
}


void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	InitItemPickup(UItemBase::StaticClass(), ItemQuantity);
}

void AItemPickup::Interact(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	if (AltCtrlMetChar != nullptr)
	{
		TakeItemPickup((AltCtrlMetChar));
	}
}

void AItemPickup::InitItemPickup(const TSubclassOf<UItemBase> ItemBaseClass, const int32 Quantity)
{
	if (!ItemRowHandle.IsNull())
	{
		const FItemData* ItemData = ItemRowHandle.GetRow<FItemData>(ItemRowHandle.RowName.ToString());
		ItemRef = NewObject<UItemBase>(this, ItemBaseClass);
		
		ItemRef->ID = ItemData->ID;
		ItemRef->ActorClass = ItemData->ActorClass;
		ItemRef->ItemType = ItemData->ItemType;
		ItemRef->ItemStats = ItemData->ItemStats;
		ItemRef->ItemText = ItemData->ItemText;
		ItemRef->InvItemStats = ItemData->InvItemStats;
		ItemRef->ItemAssets = ItemData->ItemAssets;

		ItemRef->InvItemStats.bIsStackable = ItemData->InvItemStats.StackSize > 1;

		if (Quantity <= 0)
		{
			ItemRef->SetQuantity(1);
		}
		else
		{
			ItemRef->SetQuantity(Quantity);
		}

		ItemPickupMesh->SetStaticMesh(ItemData->ItemAssets.Mesh);
		ItemPickupMesh->SetMaterial(0, ItemData->ItemAssets.Material);
		ItemPickupMesh->GetBodyInstance()->bOverrideMass = true;
		ItemPickupMesh->SetMassOverrideInKg(NAME_None,this->ItemRef->InvItemStats.Weight*30+30);

		UpdateInterData();
	}
}

void AItemPickup::InitItemOnDrop(UItemBase* ItemDrop, const int32 Quantity)
{
	ItemRef = ItemDrop;
	if (Quantity <= 0)
	{
		ItemRef->SetQuantity(1);
	}
	else
	{
		ItemRef->SetQuantity(Quantity);
	}
	ItemRef->InvItemStats.Weight = ItemDrop->GetItemSingleWeight();
	ItemRef->Inv = nullptr;
	
	ItemPickupMesh->SetStaticMesh(ItemDrop->ItemAssets.Mesh);
	ItemPickupMesh->GetBodyInstance()->bOverrideMass = true;
	ItemPickupMesh->SetMassOverrideInKg(NAME_None,this->ItemRef->InvItemStats.Weight*30+30);

	UpdateInterData();
}

void AItemPickup::TakeItemPickup(const AAltControlMethodsCharacter* User)
{
	if (!IsPendingKillPending())
	{
		if (ItemRef != nullptr)
		{
			if (UInventoryComponent* UserInv = User->GetInv())
			{
				const FItemAddRes AddRes = UserInv->AddItemHandle(ItemRef);
				
				switch (AddRes.AddActionRes)
				{
				case EItemAddRes::NoItemAdded:
					break;
				case EItemAddRes::SomeItemAdded:
					UpdateInterData();
					User->UpdateInterWidget();
					break;
				case EItemAddRes::AllItemAdded:
					Destroy();
					break;
				}
				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddRes.ResMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Hiba: Inventory null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hiba: Item referencia null!"));
		}
	}
}

void AItemPickup::UpdateInterData()
{
	InterInst.InteractableType = EInteractableType::Pickup;
	InterInst.Action = ItemRef->ItemText.InterText;
	InterInst.Name = ItemRef->ItemText.Name;
	InterInst.Quantity = ItemRef->Quantity;
	InteractableData = InterInst;
	
}
#if WITH_EDITOR
void AItemPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if (!ItemRowHandle.IsNull())
		{
			if (const FItemData* ItemData = ItemRowHandle.GetRow<FItemData>(ItemRowHandle.RowName.ToString()))
			{
				ItemPickupMesh->SetStaticMesh(ItemData->ItemAssets.Mesh);
			}
		}
	}
}
#endif