#include "AltControlMethods/UI/AmmoCounter.h"

#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"


void UAmmoCounter::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	User = Cast<AAltControlMethodsCharacter>(GetOwningPlayerPawn());
	if (User != nullptr)
	{
		InvRef = User->GetInv();
		if (InvRef != nullptr)
		{
			//Adding the redraw func to InvUpdate delegate (for broadcasting)
			InvRef->InvUpdate.AddUObject(this, &UAmmoCounter::UpdateAmmoCounter);
		}
	}
}

void UAmmoCounter::UpdateAmmoCounter() const
{
	if (User->EquippedItemBase != nullptr)
	{
		const int AmmoTotal = User->GetInv()->OwnedItemSumByFNameID(User->EquippedItemBase->ItemAssets.UsedAmmoID);
		SetAmmoText(User->EquippedItemBase->ItemStats.AmmoInMag,AmmoTotal);
	}
}

void UAmmoCounter::SetAmmoText(const int AmmoMag, const int AmmoTot) const
{
	AmmoCounterText->SetText(FText::Format(NSLOCTEXT("AmmoCounter", "AmmoCount", "{0}/{1}"), AmmoMag, AmmoTot));
}