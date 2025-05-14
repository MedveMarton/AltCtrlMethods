// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/InventoryComponent.h"

#include "AltControlMethods/Inventory/Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

UItemBase* UInventoryComponent::FindItemByFNameID(FName ItemID) const
{
	for (int i = 0; i < InvContent.Num(); ++i)
	{
		if (InvContent[i]->ID == ItemID)
		{
			return InvContent[i];
		}
	}
	return nullptr;
}

int32 UInventoryComponent::OwnedItemSumByFNameID(const FName ItemID) const
{
	int32 Sum = 0;
	for (int i = 0; i < InvContent.Num(); ++i)
	{
		if (InvContent[i]->ID == ItemID)
		{
			Sum = InvContent[i]->Quantity + Sum;
		}
	}
	return Sum;
}

UItemBase* UInventoryComponent::FindSameItem(UItemBase* Item) const
{
	if (Item != nullptr)
	{
		if (InvContent.Contains(Item))
		{
			return Item;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemById(UItemBase* Item) const
{
	if (Item != nullptr)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Res = InvContent.FindByKey(Item))
		{
			return *Res;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextIncompleteStack(UItemBase* Item) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Res = InvContent.FindByPredicate([&Item](const UItemBase* InvItem)
	{
		return InvItem->ID == Item->ID && !InvItem->IsFullStack();
	}))
	{
		return *Res;
	}
	return nullptr;
}

void UInventoryComponent::RemoveItem(UItemBase* Item)
{
	InvContent.RemoveSingle(Item);
	InvUpdate.Broadcast();
}

int32 UInventoryComponent::RemoveMultipleItems(UItemBase* Item, int32 Amount)
{
	const int32 AmountToRemove = FMath::Min(Amount, Item->Quantity);

	Item->SetQuantity(Item->Quantity - AmountToRemove);

	InvTotalWgt -= AmountToRemove * Item->GetItemSingleWeight();

	InvUpdate.Broadcast();

	return AmountToRemove;
}

void UInventoryComponent::SplitStack(UItemBase* Item, const int32 SplitAmount)
{
	if (InvContent.Num() < SlotCap)
	{
		RemoveMultipleItems(Item, SplitAmount);
		AddItem(Item, SplitAmount);
	}
}

FItemAddRes UInventoryComponent::AddItemHandle(UItemBase* Item)
{
	if (GetOwner() != nullptr)
	{
		const int32 AddAmount = Item->Quantity;

		if (Item->InvItemStats.bIsStackable)
		{
			const int32 ActAmountAdded = StackItemHandle(Item, AddAmount);
			if (ActAmountAdded == AddAmount)
			{
				return FItemAddRes::AddedAll(ActAmountAdded, FText::Format(FText::FromString("{0} {1} teljesen berakva."), Item->ItemText.Name, ActAmountAdded));
			}

			if (ActAmountAdded < AddAmount && ActAmountAdded > 0)
			{
				return FItemAddRes::AddedSome(ActAmountAdded, FText::Format(FText::FromString("{0} {1} részlegesen berakva."), Item->ItemText.Name, ActAmountAdded));
			}

			if (ActAmountAdded <= 0)
			{
				return FItemAddRes::AddedNone(FText::Format(FText::FromString("{0} nem lett berakva."), Item->ItemText.Name));
			}
		}
		else
		{
			return NonStackItemHandle(Item);
		}
	}
	return FItemAddRes::AddedNone(FText::FromString("Hiba: Semmi nem lett berakva. (Nem volt Inventory tulajdonos!)"));
}

FItemAddRes UInventoryComponent::NonStackItemHandle(UItemBase* Item)
{
	if (Item->GetItemSingleWeight() < 0)
	{
		return FItemAddRes::AddedNone(FText::Format(FText::FromString("Nem lehetett az Inventory-ba rakni: {0}. Nem megfelelő a súly."), Item->ItemText.Name));
	}
	if (InvTotalWgt + Item->GetItemSingleWeight() > GetWgtCap())
	{
		return FItemAddRes::AddedNone(FText::Format(FText::FromString("Nem lehetett az Inventory-ba rakni: {0}. Túl sok a súly."), Item->ItemText.Name));
	}

	if (InvContent.Num() >= SlotCap)
	{
		return FItemAddRes::AddedNone(FText::Format(FText::FromString("Nem lehetett az Inventory-ba rakni: {0}. Nincs elég hely."), Item->ItemText.Name));
	}

	AddItem(Item, 1);
	return FItemAddRes::AddedAll(1,FText::Format(FText::FromString("Az Inventory-ba rakva: {0}."), Item->ItemText.Name));
}

int32 UInventoryComponent::StackItemHandle(UItemBase* Item, int32 AddAmount)
{
	if (AddAmount <= 0)
	{
		return 0;
	}

	int32 AddAmountLeft = AddAmount;
	UItemBase* SameItemStackInInv = FindNextIncompleteStack(Item);

	//mar invben levo stack(ek)-be rendezes
	while (SameItemStackInInv != nullptr)
	{
		//db szamokat adnak vissza
		const int32 AmountForFullStack = CalcForFullStack(SameItemStackInInv, AddAmountLeft);
		const int32 WgtLimitAdd = CalcWgtAddAmount(SameItemStackInInv, AmountForFullStack);

		//ameddig meg lehet tobb tomeget adni az invbe
		if(WgtLimitAdd > 0)
		{
			//a stackhez adni az adhato tomegu item db-ot
			SameItemStackInInv->SetQuantity(SameItemStackInInv->Quantity + WgtLimitAdd);
			//inv tomeg allitas
			InvTotalWgt += (SameItemStackInInv->GetItemSingleWeight()*WgtLimitAdd);

			//A maradek nem hozzaadott item szam kiszamolasa
			AddAmountLeft -= WgtLimitAdd;
			//a beadott item maradekanak beallitasa
			Item->SetQuantity(AddAmountLeft);

			//ha az ossztomeg megegyezik a max tomeggel return
			if (InvTotalWgt >= WgtCap)
			{
				InvUpdate.Broadcast();
				return AddAmount - AddAmountLeft;
			}
		}
		else if (WgtLimitAdd <= 0)
		{
			//ha a maradek mar nem egyezik az eredeti osszeggel
			if (AddAmountLeft != AddAmount)
			{
				//ha mar kapott stack item-et, de az adhato item db mar nulla (tomeg miatt)
				InvUpdate.Broadcast();
				return AddAmount - AddAmountLeft;
			}
			//ha van nem teljes stack, de nem lehet adni egy db-t sem
			return 0;
		}

		if (AddAmountLeft <= 0)
		{
			//az item elfogyott az osszes meglevo stack kozott
			InvUpdate.Broadcast();
			return AddAmount;
		}

		//frissitse az elerheto stack-(ek)et, ha nincs masik azonos item stack nullptr lesz, a while leall
		SameItemStackInInv = FindNextIncompleteStack(Item);
	}

	//ha mar nincs azonos item stack az invben, de meg van adando item, es meg lehet adni
	if (InvContent.Num() < SlotCap)
	{
		//adhato item db szamolas tomeg alapjan (mint elobb)
		const int32 WgtLimitAdd = CalcWgtAddAmount(Item, AddAmount);
		if (WgtLimitAdd > 0)
		{
			//ha kevesebb hely (tomegbiras) van mint amit kene adni
			if (WgtLimitAdd < AddAmountLeft)
			{
				//a maradek adando item kiszamolasa (db szamolas)
				AddAmountLeft -= WgtLimitAdd;
				//item maradek beallitasa
				Item->SetQuantity(AddAmountLeft);

				//uj stack adasa a maradando hely (tomeg) figyelembevetelevel (az elobb levont db, adasa az invbe) (uj memoriaba a masolt reszletet, hogy a maradek, es az invbe adott item ne azonos pointerrel mutasson stb..)
				AddItem(Item->CreateItemCopy(), WgtLimitAdd);
				return AddAmount - AddAmountLeft;
			}
			//itt az egesz kerul be, nem kell copy, egy helyen lesz
			AddItem(Item, AddAmountLeft);
			return AddAmount;
		}
		//ha van szabad hely, de tobb tomeg mar nem lehet
		InvUpdate.Broadcast();
		return AddAmount - AddAmountLeft;
	}
	//nem volt meglevo stack, nincs mar slot
	return 0;
}

int32 UInventoryComponent::CalcWgtAddAmount(UItemBase* Item, int32 AddAmount)
{
	const int32 WgtMaxAddAmount = FMath::FloorToInt((GetWgtCap() - InvTotalWgt) / Item->GetItemSingleWeight());

	if (WgtMaxAddAmount >= AddAmount)
	{
		return AddAmount;
	}
	return WgtMaxAddAmount;
}

int32 UInventoryComponent::CalcForFullStack(UItemBase* Item, int32 AddAmount)
{
	const int32 AmountForFullStack = Item->InvItemStats.StackSize - Item->Quantity;

	if (AddAmount <= AmountForFullStack)
	{
		return AddAmount;
	}
	return AmountForFullStack;
}

void UInventoryComponent::AddItem(UItemBase* Item, const int32 AddAmount)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		NewItem = Item->CreateItemCopy();
	}

	NewItem->Inv = this;
	NewItem->SetQuantity(AddAmount);
	InvContent.Add(NewItem);
	InvTotalWgt += NewItem->GetItemStackWeight();
	InvUpdate.Broadcast();
}