// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/Inventory/Items/WeaponActorBase.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/WeaponMeshComps/TP_FirearmComponent.h"
#include "RangedWeaponActor.generated.h"

class UItemBase;
class AAltControlMethodsCharacter;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API ARangedWeaponActor : public AWeaponActorBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* RFireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* RFireAction;

public:
	
	ARangedWeaponActor();
	
	UPROPERTY()
	UItemBase* ItemBaseRef;

	UPROPERTY()
	UStaticMeshComponent* SkelMeshComp;

	AAltControlMethodsCharacter* GetCharacter() const
	{
		return this->Character;
	}

	/** Projectile class*/
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	/** Firing Sound*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** Fire Anim*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;
	
	UPROPERTY()
	float ReloadTime;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void AttachWeapon(AAltControlMethodsCharacter* TargetCharacter) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();

	virtual void InitWeaponActor(UItemBase* ItemRef) override;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	UPROPERTY()
	AAltControlMethodsCharacter* Character;
};
