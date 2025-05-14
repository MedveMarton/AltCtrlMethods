// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActorBase.h"
#include "ActorFactories/ActorFactoryClass.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/Items/WeaponClasses/WeaponMeshComps/TP_FirearmComponent.h"
#include "AltControlMethods/Data/ItemDataStructs.h"
#include "ItemBase.generated.h"


class ARangedWeaponMeshActor;
class AStaticMeshActor;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUse, AAltControlMethodsCharacter*, UsingCharacter);


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALTCONTROLMETHODS_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUse OnUse;

	UPROPERTY()
	UInventoryComponent* Inv;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TSubclassOf<AWeaponActorBase> ActorClass;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	AWeaponActorBase* ActorClassRef;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStats ItemStats;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemText ItemText;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FInvItemStats InvItemStats;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssets ItemAssets;

	bool bIsCopy;

	bool bIsPickup;


	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const {return Quantity * InvItemStats.Weight;};
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const {return InvItemStats.Weight;};

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullStack() const {return Quantity == InvItemStats.StackSize;};

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuant);

	UFUNCTION(Category = "Item")
	virtual void Use(AAltControlMethodsCharacter* Character);

	void ResetItemFlags();

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
	
};
