
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvSlot.generated.h"

class UUsageWidget;
class UImage;
class UBorder;
class UTextBlock;
class UInvItemHover;
class UDraggedItem;
class UItemBase;
/**
 * 
 */
UCLASS()
class ALTCONTROLMETHODS_API UInvSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetItemRef(UItemBase* Item)
	{
		ItemRef = Item;
	}

	UItemBase* GetItemRef() const
	{
		return ItemRef;
	}

protected:

	UPROPERTY(EditDefaultsOnly, Category ="Inv Slot")
	TSubclassOf<UDraggedItem> DraggedItemClass;

	UPROPERTY(EditDefaultsOnly, Category ="Inv Slot")
	TSubclassOf<UInvItemHover> ItemHoverClass;

	UPROPERTY(EditDefaultsOnly, Category ="Inv Slot")
	TSubclassOf<UUsageWidget> ItemUsageClass;
	
	UPROPERTY(VisibleAnywhere, Category="Inv Slot")
	UItemBase* ItemRef;

	UPROPERTY(VisibleAnywhere, Category="Inv Slot", meta=(BindWidget))
	UBorder* Border;
	UPROPERTY(VisibleAnywhere, Category="Inv Slot", meta=(BindWidget))
	UImage* Icon;
	UPROPERTY(VisibleAnywhere, Category="Inv Slot", meta=(BindWidget))
	UTextBlock* Quantity;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;


	
};
