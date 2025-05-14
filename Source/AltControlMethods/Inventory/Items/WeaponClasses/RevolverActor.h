// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/RangedWeaponActor.h"
#include "WeaponMeshComps/TP_FirearmComponent.h"
#include "RevolverActor.generated.h"

/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API ARevolverActor : public ARangedWeaponActor
{
	GENERATED_BODY()

public:
	ARevolverActor();

	virtual void InitWeaponActor(UItemBase* Item) override;
	virtual void AttachWeapon(AAltControlMethodsCharacter* TargetCharacter) override;
	virtual void DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;

	UPROPERTY()
	UTP_FirearmComponent* SkelMeshSubComp;
};
