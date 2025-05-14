// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/MeleeWeaponActor.h"
#include "ShortSwordActor.generated.h"

/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API AShortSwordActor : public AMeleeWeaponActor
{
	GENERATED_BODY()

	virtual void AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;
	virtual void DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;
};
