

#include "AltControlMethods/Inventory/Items/WeaponClasses/RangedWeaponActor.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "AltControlMethods/Inventory/Items/ProjectileBase.h"
#include "AltControlMethods/UI/AltCtrlMetHUD.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ARangedWeaponActor::ARangedWeaponActor()
{

	PrimaryActorTick.bCanEverTick = true;
	SkelMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SkelMeshComp");
	SkelMeshComp->SetSimulatePhysics(false);
	SetRootComponent(SkelMeshComp);
	SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(false);
	
	ReloadTime = 0;
}


void ARangedWeaponActor::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
}

void ARangedWeaponActor::InitWeaponActor(UItemBase* ItemRef)
{
};

void ARangedWeaponActor::AttachWeapon(AAltControlMethodsCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid
	if (Character == nullptr)
	{
		return;
	}

	Character->GetHUD()->DisplayAmmoCounter();
	
	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
/**
	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(RFireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(RFireAction, ETriggerEvent::Triggered, this, &ARangedWeaponActor::Fire);
		}
	}
	**/
}

void ARangedWeaponActor::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);

	Character = AltCtrlMetChar;

	Character->GetHUD()->HideAmmoCounter();

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(DetachRules);
	this->Destroy();

	/**
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(RFireMappingContext);
		}
	}
	**/
}

void ARangedWeaponActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}
/**
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(RFireMappingContext);
		}
	}
	**/
}