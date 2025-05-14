// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeaponActorExp.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/ShortSwordActor.h"
#include "WeaponMeshComps/TP_MeleeComponentExp.h"
#include "ShortSwordActorExp.generated.h"

class UItemBase;
class AAltControlMethodsCharacter;

/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API AShortSwordActorExp : public AMeleeWeaponActorExp
{
	GENERATED_BODY()
	
	virtual void AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;
	virtual void DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;
};
