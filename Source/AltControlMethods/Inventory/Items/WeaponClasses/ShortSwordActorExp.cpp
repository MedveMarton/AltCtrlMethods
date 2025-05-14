
#include "AltControlMethods/Inventory/Items/WeaponClasses/ShortSwordActorExp.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"

void AShortSwordActorExp::AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::AttachWeapon(AltCtrlMetChar);
	
	// switch bHasOhSword so the animation blueprint can switch to another animation set
	GetCharacter()->SetHasOhSword(true);
	SkelMeshSubComp->AttachWeapon(GetCharacter());
}

void AShortSwordActorExp::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);
	
	// switch bHasOhSword so the animation blueprint can switch to another animation set
	GetCharacter()->SetHasOhSword(false);
	SkelMeshSubComp->DetachWeapon(GetCharacter());
}