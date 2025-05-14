// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FInvUpdate);

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddRes: uint8
{
	NoItemAdded UMETA(DisplayName = "No item added"),
	SomeItemAdded UMETA(DisplayName = "Some items added"),
	AllItemAdded UMETA(DisplayName = "All items added")
};

USTRUCT(BlueprintType)
struct FItemAddRes
{
	GENERATED_BODY()
	
	FItemAddRes() : AmountAdded(0), AddActionRes(EItemAddRes::NoItemAdded), ResMessage(FText::GetEmpty())
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Res")
	int32 AmountAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Res")
	EItemAddRes AddActionRes;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Res")
	FText ResMessage;

	static FItemAddRes AddedNone(const FText& Msg)
	{
		FItemAddRes AddedNoneRes;
		AddedNoneRes.AmountAdded = 0;
		AddedNoneRes.AddActionRes = EItemAddRes::NoItemAdded;
		AddedNoneRes.ResMessage = Msg;
		return AddedNoneRes;
	};
	static FItemAddRes AddedSome(const int32 AmountAdded, const FText& Msg)
	{
		FItemAddRes AddedSomeRes;
		AddedSomeRes.AmountAdded = AmountAdded;
		AddedSomeRes.AddActionRes = EItemAddRes::SomeItemAdded;
		AddedSomeRes.ResMessage = Msg;
		return AddedSomeRes;
	};
	static FItemAddRes AddedAll(const int32 AmountAdded, const FText& Msg)
	{
		FItemAddRes AddedAllRes;
		AddedAllRes.AmountAdded = AmountAdded;
		AddedAllRes.AddActionRes = EItemAddRes::AllItemAdded;
		AddedAllRes.ResMessage = Msg;
		return AddedAllRes;
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALTCONTROLMETHODS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FInvUpdate InvUpdate;
	
	UInventoryComponent();

	UFUNCTION(Category = "Inv")
	UItemBase* FindItemByFNameID(FName ItemID) const;
	UFUNCTION(Category = "Inv")
	int OwnedItemSumByFNameID(FName ItemID) const;
	UFUNCTION(Category = "Inv")
	UItemBase* FindSameItem(UItemBase* Item) const;
	UFUNCTION(Category = "Inv")
	UItemBase* FindNextItemById(UItemBase* Item) const;
	UFUNCTION(Category = "Inv")
	UItemBase* FindNextIncompleteStack(UItemBase* Item) const;

	UFUNCTION(Category = "Inv")
	FItemAddRes AddItemHandle(UItemBase* Item);

	UFUNCTION(Category = "Inv")
	void RemoveItem(UItemBase* Item);
	UFUNCTION(Category = "Inv")
	int32 RemoveMultipleItems(UItemBase* Item, int32 Amount);
	UFUNCTION(Category = "Inv")
	void SplitStack(UItemBase* Item, const int32 SplitAmount);

	UFUNCTION(Category = "Inv")
	float GetInvTotalWgt() const { return InvTotalWgt;};
	UFUNCTION(Category = "Inv")
	float GetWgtCap() const { return WgtCap;};
	UFUNCTION(Category = "Inv")
	int32 GetSlotCap() const { return SlotCap;};
	UFUNCTION(Category = "Inv")
	TArray<UItemBase*> GetInvContent() const { return InvContent;};

	UFUNCTION(Category = "Inv")
	void SetSlotCap(const int32 SlotCount) { SlotCap = SlotCount;};
	UFUNCTION(Category = "Inv")
	void SetWgtCap(const float WeightCap) { WgtCap = WeightCap;};
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Inv")
	float InvTotalWgt;
	UPROPERTY(VisibleAnywhere, Category = "Inv")
	TArray<TObjectPtr<UItemBase>> InvContent;
	UPROPERTY(EditInstanceOnly, Category = "Inv")
	int32 SlotCap;
	UPROPERTY(EditInstanceOnly, Category = "Inv")
	float WgtCap;

	FItemAddRes NonStackItemHandle(UItemBase* Item);
	int32 StackItemHandle(UItemBase* Item, int32 AddAmount);
	int32 CalcWgtAddAmount(UItemBase* Item, int32 AddAmount);
	int32 CalcForFullStack(UItemBase* Item, int32 AddAmount);

	void AddItem(UItemBase* Item, const int32 AddAmount);
};