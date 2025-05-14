// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "WeaponActorBase.generated.h"

class AAltControlMethodsCharacter;
class UItemBase;
/**
 * 
 */
UCLASS(Abstract)
class ALTCONTROLMETHODS_API AWeaponActorBase : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	virtual void InitWeaponActor(UItemBase* Item);
	virtual void AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar);
	virtual void DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar);
};
