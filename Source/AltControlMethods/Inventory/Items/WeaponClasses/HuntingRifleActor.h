#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/RangedWeaponActor.h"
#include "WeaponMeshComps/TP_FirearmComponent.h"
#include "HuntingRifleActor.generated.h"


/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API AHuntingRifleActor : public ARangedWeaponActor
{
	GENERATED_BODY()

public:
	AHuntingRifleActor();

	virtual void InitWeaponActor(UItemBase* Item) override;
	virtual void AttachWeapon(AAltControlMethodsCharacter* TargetCharacter) override;
	virtual void DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar) override;

	UPROPERTY()
	UTP_FirearmComponent* SkelMeshSubComp;
};