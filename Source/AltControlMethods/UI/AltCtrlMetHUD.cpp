// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/UI/AltCtrlMetHUD.h"
#include "AltControlMethods/UI/UserMenuWidget.h"
#include "AltControlMethods/UI/InterWidget.h"

void AAltCtrlMetHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UserMenuClass != nullptr)
	{
		UserMenuWidget = CreateWidget<UUserMenuWidget>(GetWorld(), UserMenuClass);
		UserMenuWidget->AddToViewport(1);
		UserMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UserMenuClass != nullptr)
	{
		InterWidget = CreateWidget<UInterWidget>(GetWorld(), InterWidgetClass);
		InterWidget->AddToViewport(-1);
		InterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (GenCharHUDClass != nullptr)
	{
		GeneralCharHUDWidget = CreateWidget<UGeneralCharHUD>(GetWorld(), GenCharHUDClass);
		GeneralCharHUDWidget->AddToViewport(-2);
		GeneralCharHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (AmmoCounterClass != nullptr)
	{
		AmmoCounterWidget = CreateWidget<UAmmoCounter>(GetWorld(), AmmoCounterClass);
		AmmoCounterWidget->AddToViewport(-3);
		AmmoCounterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

AAltCtrlMetHUD::AAltCtrlMetHUD()
{
}

void AAltCtrlMetHUD::DisplayMenu()
{
	if (UserMenuWidget != nullptr)
	{
		IsMenuVisible = true;
		UserMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAltCtrlMetHUD::HideMenu()
{
	if (UserMenuWidget != nullptr)
	{
		IsMenuVisible = false;
		UserMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AAltCtrlMetHUD::ToggleMenu()
{
	if (IsMenuVisible)
	{
		HideMenu();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AAltCtrlMetHUD::ShowInterWidget() const
{
	if (InterWidget != nullptr)
	{
		InterWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAltCtrlMetHUD::HideInterWidget() const
{
	if (InterWidget != nullptr)
	{
		InterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AAltCtrlMetHUD::UpdateInterWidget(const FInteractableData* InteractableData) const
{
	if (InterWidget != nullptr)
	{
		if (InterWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InterWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InterWidget->UpdateWidget(InteractableData);
	}
}

void AAltCtrlMetHUD::DisplayAmmoCounter() const
{
	if (AmmoCounterWidget != nullptr)
	{
		AmmoCounterWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAltCtrlMetHUD::UpdateAmmoCounter(const int AmmoInMag, const int AmmoTotal) const
{
	if (AmmoCounterWidget != nullptr)
	{
		AmmoCounterWidget->SetAmmoText(AmmoInMag, AmmoTotal);
	}
}

void AAltCtrlMetHUD::HideAmmoCounter() const
{
	if (AmmoCounterWidget != nullptr)
	{
		AmmoCounterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}