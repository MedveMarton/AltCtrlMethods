// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/Inventory/Items/WeaponActorBase.h"
#include "WeaponMeshComps/TP_MeleeComponentExp.h"
#include "MeleeWeaponActorExp.generated.h"

/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API AMeleeWeaponActorExp : public AWeaponActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeleeWeaponActorExp();
	
	UPROPERTY()
	UItemBase* ItemBaseRef;

	UPROPERTY()
	UStaticMeshComponent* SkelMeshComp;

	UPROPERTY()
	UTP_MeleeComponentExp* SkelMeshSubComp;

	AAltControlMethodsCharacter* GetCharacter() const
	{
		return this->Character;
	}
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void InitWeaponActor(UItemBase* ItemRef) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;

private:
	/** The Character holding this weapon*/
	UPROPERTY()
	AAltControlMethodsCharacter* Character;
};
