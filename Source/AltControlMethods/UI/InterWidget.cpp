// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/UI/InterWidget.h"

#include "AltControlMethods/Interfaces/InteractionInterface.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UInterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InterProgBar->PercentDelegate.BindUFunction(this,"UpdateInterProg");
}

void UInterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressTxt->SetText(FText::FromString("Nyomd:"));
	CurrentInterDur = 0.0f;
}

void UInterWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup:
		KeyPressTxt->SetText(FText::FromString("Nyomd:"));
		InterProgBar->SetVisibility(ESlateVisibility::Collapsed);

		if (InteractableData->Quantity == 1)
		{
			QuantityTxt->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityTxt->SetText(FText::Format(NSLOCTEXT("InterWidget", "QuantityTxt", "({0})"), InteractableData->Quantity));

			QuantityTxt->SetVisibility(ESlateVisibility::Visible);
		}
		break;
	case EInteractableType::InterObject:
		break;
	case EInteractableType::Container:
		break;
	case EInteractableType::NPC:
		break;
	}
	ActionTxt->SetText(InteractableData->Action);
	NameTxt->SetText(InteractableData->Name);
}

float UInterWidget::UpdateInterProg()
{
	return 0.0f;
}