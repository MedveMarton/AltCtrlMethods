// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/WeaponClasses/LongSwordActor.h"

#include "AltControlMethods/AltControlMethodsCharacter.h"

void ALongSwordActor::AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::AttachWeapon(AltCtrlMetChar);

	// switch bHasSword so the animation blueprint can switch to another animation set
	GetCharacter()->SetHasSword(true);
	SkelMeshSubComp->AttachWeapon(GetCharacter());
}

void ALongSwordActor::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);

	GetCharacter()->SetHasSword(false);
	SkelMeshSubComp->DetachWeapon(GetCharacter());
}