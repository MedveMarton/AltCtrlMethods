// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/Inventory/Items/WeaponActorBase.h"
#include "GameFramework/Actor.h"
#include "WeaponMeshComps/TP_MeleeComponent.h"
#include "MeleeWeaponActor.generated.h"

class UItemBase;
class AAltControlMethodsCharacter;

UCLASS()
class ALTCONTROLMETHODS_API AMeleeWeaponActor : public AWeaponActorBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AMeleeWeaponActor();
	
	UPROPERTY()
	UItemBase* ItemBaseRef;

	UPROPERTY()
	UStaticMeshComponent* SkelMeshComp;

	UPROPERTY()
	UTP_MeleeComponent* SkelMeshSubComp;

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