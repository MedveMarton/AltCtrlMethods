#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/WeaponMeshComps/TP_MeleeComponent.h"
#include "AltControlMethods/Data/EDirection.h"
#include "TP_MeleeComponentExp.generated.h"

UCLASS()
class ALTCONTROLMETHODS_API UTP_MeleeComponentExp : public UTP_MeleeComponent
{
	GENERATED_BODY()


	virtual void Fire() override;
	
	EDirection LastDir;
};