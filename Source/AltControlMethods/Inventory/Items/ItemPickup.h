// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Interfaces/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "ItemPickup.generated.h"

class UDataTable;
class UItemBase;

UCLASS()
class ALTCONTROLMETHODS_API AItemPickup : public AActor, public IInteractionInterface
{
public:

	virtual void Interact(AAltControlMethodsCharacter* AltCtrlMetChar) override;

private:
	GENERATED_BODY()

public:	
	AItemPickup();

	void InitItemPickup(const TSubclassOf<UItemBase> ItemBaseClass, const int32 Quantity);
	void InitItemOnDrop(UItemBase* ItemDrop, const int32 Quantity);

	UItemBase* GetItemData() {return ItemRef;};
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "ItemPickup | Assets")
	UStaticMeshComponent* ItemPickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "ItemPickup | Item Ref")
	UItemBase* ItemRef;

	UPROPERTY(EditInstanceOnly, Category = "ItemPickup | Item Data")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "ItemPickup | Interact")
	FInteractableData InterInst;

	UPROPERTY(EditInstanceOnly, Category = "ItemPickup | Item Data")
	FDataTableRowHandle ItemRowHandle;

	virtual void BeginPlay() override;

	void TakeItemPickup(const AAltControlMethodsCharacter* User);

	void UpdateInterData();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
