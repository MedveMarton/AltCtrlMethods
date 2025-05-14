

#include "AltControlMethods/UI/InvSlot.h"

#include "DraggedItem.h"
#include "InvItemHover.h"
#include "ItemDragDropOp.h"
#include "UsageWidget.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UInvSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ItemHoverClass != nullptr)
	{
		UInvItemHover* ItemHover = CreateWidget<UInvItemHover>(this, ItemHoverClass);
		ItemHover->InvSlotHovered = this;
		SetToolTip(ItemHover);
	}
}

void UInvSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemRef != nullptr)
	{
		switch (ItemRef->ItemType) {
		case EItemType::General:
			Border->SetBrushColor(FLinearColor(0.8, 1, 1, 1));
			break;
		case EItemType::RangedWeapon:
			Border->SetBrushColor(FLinearColor(0.88, 0.5, 0.5 ));
			break;
		case EItemType::MeleeWeapon:
			Border->SetBrushColor(FLinearColor(0.88, 0.5, 0.5 ));
			break;
		case EItemType::Consumable:
			Border->SetBrushColor(FLinearColor(0.75, 0.7, 1, 1));
			break;
		case EItemType::Ammunition:
			Border->SetBrushColor(FLinearColor(1, 0.73, 0.6, 1));
			break;
		default: ;
		}

		Icon->SetBrushFromTexture(ItemRef->ItemAssets.InvIcon);

		if (ItemRef->InvItemStats.bIsStackable)
		{
			Quantity->SetText(FText::AsNumber(ItemRef->Quantity));
		}
		else
		{
			Quantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInvSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Show Usage"));
		
		if (ItemUsageClass != nullptr)
		{
			UUsageWidget* UsageWidget = CreateWidget<UUsageWidget>(this, ItemUsageClass);
			UsageWidget->InvSlotUsed = this;
			
			UsageWidget->AddToViewport(2);
			float MouseX;
			float MouseY;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MouseX, MouseY);
			FVector2D MouseLoc(MouseX,MouseY);
			UsageWidget->SetPositionInViewport(MouseLoc);
		}
		
		return Reply.Handled();
	}
	
	return Reply.Unhandled();
}

void UInvSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInvSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DraggedItemClass != nullptr)
	{
		const TObjectPtr<UDraggedItem> DraggedItem = CreateWidget<UDraggedItem>(this, DraggedItemClass);
		DraggedItem->Icon->SetBrushFromTexture(ItemRef->ItemAssets.InvIcon);
		DraggedItem->Border->SetBrushColor(Border->GetBrushColor());
		DraggedItem->Quantity->SetText(FText::AsNumber(ItemRef->Quantity));

		UItemDragDropOp*  DraggedItemOp = NewObject<UItemDragDropOp>();
		DraggedItemOp->SourceItem = ItemRef;
		DraggedItemOp->SourceInv = ItemRef->Inv;

		DraggedItemOp->DefaultDragVisual = DraggedItem;
		DraggedItemOp->Pivot = EDragPivot::MouseDown;

		OutOperation = DraggedItemOp;
	}
}

bool UInvSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}