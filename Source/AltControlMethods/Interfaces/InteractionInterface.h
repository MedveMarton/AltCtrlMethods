// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AAltControlMethodsCharacter;

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	InterObject UMETA(DisplayName = "InterObject"),
	Container UMETA(DisplayName = "Container"),
	NPC UMETA(DisplayName = "NPC")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()
	FInteractableData() : InteractableType(EInteractableType::Pickup), Name(FText::GetEmpty()), Action(FText::GetEmpty()), Quantity(0), InterDuration(0.0f)
	{
		
	};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;
	
	UPROPERTY(EditInstanceOnly)
	FText Name;
	
	UPROPERTY(EditInstanceOnly)
	FText Action;
	
	UPROPERTY(EditInstanceOnly)
	int32 Quantity;
	
	UPROPERTY(EditInstanceOnly)
	float InterDuration;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALTCONTROLMETHODS_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginInteraction();
	virtual void EndInteraction();
	virtual void Interact(AAltControlMethodsCharacter* AltCtrlMetChar);

	FInteractableData InteractableData;
};
