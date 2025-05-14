#include "AltControlMethodsCharacter.h"
#include "CtrlMethodsGameUserSettings.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Items/ItemBase.h"
#include "Inventory/Items/ItemPickup.h"
#include "UI/AltCtrlMetHUD.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);



AAltControlMethodsCharacter::AAltControlMethodsCharacter()
{
	bHasSword = false;
	bHasOhSword = false;
	bHasPistol = false;
	bIsCrouching = false;
	bIsSprinting = false;
	bIsAiming = false;
	
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	BaseEyeHeight = 63.0f;
	InteractionCheckFreq = 0.1;
	InteractionCheckDist = 205.0f;
	LookDirThreshold = 0.6f;
	LastDirection = EDirection::Up;

	UserInv = CreateDefaultSubobject<UInventoryComponent>(TEXT("UserInv"));
	UserInv->SetSlotCap(32);
	UserInv->SetWgtCap(64.0f);
	
}

void AAltControlMethodsCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


	HUD = Cast<AAltCtrlMetHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

//Input

void AAltControlMethodsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAltControlMethodsCharacter::Move);

		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAltControlMethodsCharacter::Look);


		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAltControlMethodsCharacter::BeginInteract);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AAltControlMethodsCharacter::EndInteract);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AAltControlMethodsCharacter::StartCrouch);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AAltControlMethodsCharacter::EndCrouch);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AAltControlMethodsCharacter::StartSprint);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAltControlMethodsCharacter::EndSprint);

		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &AAltControlMethodsCharacter::StartAiming);
		
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &AAltControlMethodsCharacter::EndAiming);

		EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &AAltControlMethodsCharacter::ToggleMenu);

		EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Started, this, &AAltControlMethodsCharacter::HideInvIfVis);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AAltControlMethodsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAltControlMethodsCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);

        if (UCtrlMethodsGameUserSettings::GetCtrlMethodsGameUserSettings()->GetMethodIndex() == 1)
        {
        	// Use input values themselves to determine movement
        	float RightMovement = LookAxisVector.X; // Horizontal camera movement
        	float UpMovement = -LookAxisVector.Y;   // Flipped vertical movement
    	
        	EDirection CurrentDirection = LastDirection;

        	// Determine movement direction based on input
        	if (FMath::Abs(RightMovement) > LookDirThreshold*0.5 && FMath::Abs(UpMovement) > LookDirThreshold*0.5)
        	{
        		if (RightMovement < 0 && UpMovement > 0)
        			CurrentDirection = EDirection::Up_Left;
        		else if (RightMovement < 0 && UpMovement < 0)
        			CurrentDirection = EDirection::Down_Left;
        		else if (RightMovement > 0 && UpMovement > 0)
        			CurrentDirection = EDirection::Up_Right;
        		else if (RightMovement > 0 && UpMovement < 0)
        			CurrentDirection = EDirection::Down_Right;
        	}
        	else if (UpMovement > LookDirThreshold)
        	{
        		CurrentDirection = EDirection::Up;
        	}
        	else if (RightMovement < -LookDirThreshold)
        		{
        			CurrentDirection = EDirection::Left;
        		}
        	else if (RightMovement > LookDirThreshold)
        		{
        			CurrentDirection = EDirection::Right;
        		}
        	if (CurrentDirection != LastDirection)
        	{
        		LastDirection = CurrentDirection;
        		//Only enable for debugging!
        		UE_LOG(LogTemp, Warning, TEXT("%s"), *DirectionToString(LastDirection));
        	}
        }
    }
}


void AAltControlMethodsCharacter::SetHasPistol(const bool bNewHasPistol)
{
	bHasPistol = bNewHasPistol;
}

void AAltControlMethodsCharacter::SetHasRifle(const bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AAltControlMethodsCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AAltControlMethodsCharacter::SetHasSword(const bool bNewHasSword)
{
	bHasSword = bNewHasSword;
}

void AAltControlMethodsCharacter::SetHasOhSword(const bool bNewHasSword)
{
	bHasOhSword = bNewHasSword;
}

bool AAltControlMethodsCharacter::GetHasSword() const
{
	return bHasSword;
}


void AAltControlMethodsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFreq)
	{
		DoInteractionCheck();
	}
}

FString AAltControlMethodsCharacter::DirectionToString(const EDirection Direction)
{
	switch (Direction)
	{
	case EDirection::Up_Left:    return "Up-Left";
	case EDirection::Down_Left:  return "Down-Left";
	case EDirection::Up_Right:   return "Up-Right";
	case EDirection::Down_Right: return "Down-Right";
	case EDirection::Left:       return "Left";
	case EDirection::Right:      return "Right";
	default:                     return "Up";
	}
}

void AAltControlMethodsCharacter::DoInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDist)};

	
	//for visible interaction trace debugging
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Emerald, false, 1.0f, 0, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult TraceHit;

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			

			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());
				return;
			}

			if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}
	NoInteractableFound();
}

void AAltControlMethodsCharacter::FoundInteractable(AActor* NewInterable)
{
	if(IsInteracting())
	{
		EndInteract();
	}
	if (InteractionData.CurrentInteractable != nullptr)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
	}
	InteractionData.CurrentInteractable = NewInterable;
	TargetInteractable = NewInterable;

	HUD->UpdateInterWidget(&TargetInteractable->InteractableData);
}

void AAltControlMethodsCharacter::UpdateInterWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInterWidget(&TargetInteractable->InteractableData);
	}
}

void AAltControlMethodsCharacter::BeginInteract()
{
	DoInteractionCheck();

	if (InteractionData.CurrentInteractable != nullptr)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteraction();
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InterDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Inter, this, &AAltControlMethodsCharacter::Interact, TargetInteractable->InteractableData.InterDuration, false);
			}
		}
	}
}

void AAltControlMethodsCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Inter);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteraction();
	}
}

void AAltControlMethodsCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Inter);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
	
}

void AAltControlMethodsCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Inter);
	}

	if (InteractionData.CurrentInteractable != nullptr)
	{
		HUD->HideInterWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AAltControlMethodsCharacter::DropItem(UItemBase* Item, const int32 Quantity)
{
	if (UserInv->FindSameItem(Item) != nullptr)
	{
		if (MainWepEquipped != nullptr && MainWepEquipped == Item->ActorClassRef)
		{
			MainWepEquipped->DetachWeapon(this);
			this->OnEquipAnimEvent.Broadcast();
			
			MainWepEquipped = nullptr;
			Item->ActorClassRef = nullptr;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		const FVector SpawnLoc{GetActorLocation() + (GetActorForwardVector() * 85.0)};
		const FTransform SpawnTransform(GetActorRotation(), SpawnLoc);

		const int32 RemovedQuant = UserInv->RemoveMultipleItems(Item, Quantity);

		AItemPickup* DroppedPickup = GetWorld()->SpawnActor<AItemPickup>(AItemPickup::StaticClass(), SpawnTransform, SpawnParams);

		DroppedPickup->InitItemOnDrop(Item, RemovedQuant);
		
	}
}

void AAltControlMethodsCharacter::EquipRangedItem(UItemBase* Item)
{
	if (UserInv->FindSameItem(Item) != nullptr && Item->ActorClass != nullptr)
	{
		if (!MainWepEquipped)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.bNoFail = true;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			const FVector SpawnLoc{GetActorLocation() + (GetActorForwardVector())};
			const FTransform SpawnTransform(GetActorRotation(), SpawnLoc);
		
			AWeaponActorBase* EquippedItem = GetWorld()->SpawnActor<AWeaponActorBase>(Item->ActorClass, SpawnTransform, SpawnParams);
			//TODO init in def constructor + setup attachment here
			EquippedItem->InitWeaponActor(Item);
			EquippedItem->AttachWeapon(this);
			this->OnEquipAnimEvent.Broadcast();
			
			MainWepEquipped = EquippedItem;
			Item->ActorClassRef = MainWepEquipped;
			EquippedItemBase = Item;
		}
		else if(MainWepEquipped == Item->ActorClassRef)
		{
			MainWepEquipped->DetachWeapon(this);
			this->OnEquipAnimEvent.Broadcast();
			
			MainWepEquipped = nullptr;
			Item->ActorClassRef = nullptr;
			EquippedItemBase = nullptr;
		}
		else
		{
			MainWepEquipped->DetachWeapon(this);
			
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.bNoFail = true;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			const FVector SpawnLoc{GetActorLocation() + (GetActorForwardVector())};
			const FTransform SpawnTransform(GetActorRotation(), SpawnLoc);
		
			AWeaponActorBase* EquippedItem = GetWorld()->SpawnActor<AWeaponActorBase>(Item->ActorClass, SpawnTransform, SpawnParams);
			//TODO init in def constructor + setup attachment here
			EquippedItem->InitWeaponActor(Item);
			EquippedItem->AttachWeapon(this);
			this->OnEquipAnimEvent.Broadcast();
		
			MainWepEquipped = EquippedItem;
			Item->ActorClassRef = MainWepEquipped;
			EquippedItemBase = Item;
		}
		UserInv->InvUpdate.Broadcast();
	}
}

void AAltControlMethodsCharacter::StartCrouch()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(50.f);
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	ACharacter::Crouch();
	bIsCrouching = true;
}

void AAltControlMethodsCharacter::EndCrouch()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	ACharacter::UnCrouch();
	bIsCrouching = false;
}

void AAltControlMethodsCharacter::StartSprint()
{
	if (auto CharMovement = GetCharacterMovement())
	{
		CharMovement->MaxWalkSpeed = 1000.f;
		bIsSprinting = true;
	}
}


void AAltControlMethodsCharacter::EndSprint()
{
	if (auto CharMovement = GetCharacterMovement())
	{
		CharMovement->MaxWalkSpeed = 600.f;
		bIsSprinting = false;
	}
}

void AAltControlMethodsCharacter::StartAiming()
{
	if (auto FirstPersonCamera = GetFirstPersonCameraComponent())
	{
		FirstPersonCamera->SetFieldOfView(75.f);
		bIsAiming = true;
	}
}

void AAltControlMethodsCharacter::EndAiming()
{
	if (auto FirstPersonCamera = GetFirstPersonCameraComponent())
	{
		FirstPersonCamera->SetFieldOfView(90.f);
		bIsAiming = false;
	}
}

void AAltControlMethodsCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void AAltControlMethodsCharacter::HideInvIfVis()
{
	HUD->HideMenu();
}