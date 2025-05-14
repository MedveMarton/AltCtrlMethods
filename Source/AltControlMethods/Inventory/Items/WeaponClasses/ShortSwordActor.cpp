// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/WeaponClasses/ShortSwordActor.h"

#include "AltControlMethods/AltControlMethodsCharacter.h"

void AShortSwordActor::AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::AttachWeapon(AltCtrlMetChar);

	// switch bHasOhSword so the animation blueprint can switch to another animation set
	GetCharacter()->SetHasOhSword(true);
	SkelMeshSubComp->AttachWeapon(GetCharacter());
}

void AShortSwordActor::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);

	// switch bHasOhSword so the animation blueprint can switch to another animation set
	GetCharacter()->SetHasOhSword(false);
	SkelMeshSubComp->DetachWeapon(GetCharacter());
}
