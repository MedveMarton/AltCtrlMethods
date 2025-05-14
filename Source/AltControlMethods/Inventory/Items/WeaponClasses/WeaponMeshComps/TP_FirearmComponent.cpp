#include "TP_FirearmComponent.h"
#include "AltcontrolMethods/AltControlMethodsCharacter.h"
#include "AltcontrolMethods/Inventory/Items/ProjectileBase.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AltControlMethods/Inventory/InventoryComponent.h"
#include "AltControlMethods/UI/AltCtrlMetHUD.h"

// Sets default values for this component's properties
UTP_FirearmComponent::UTP_FirearmComponent(): ItemBaseRef(nullptr), AmmoSumTotal(0), IsReloading(false),
                                              FireSound(nullptr),
                                              ReloadAnimation(nullptr),
                                              Character(nullptr)
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(55.0f, 15.0f, 5.0f);

	static ConstructorHelpers::FClassFinder<AProjectileBase> ProjectileBPClass(
		TEXT("/Game/CtrlMethodsDemo/Blueprints/BP_ProjectileBase.BP_ProjectileBase_C"));
	if (ProjectileBPClass.Succeeded())
	{
		ProjectileClass = ProjectileBPClass.Class;
	}
	//FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/FPWeapons/Audio/GShot2.GShot2"));

	/*
	 *static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageFinder(
		TEXT("/Game/FirstPersonArms/Animations/FP_Rifle_Shoot_Montage.FP_Rifle_Shoot_Montage"));
	if (AnimMontageFinder.Succeeded())
	{
		FireAnimation = AnimMontageFinder.Object;
	};
	*/

	const FString MappingContextPath = TEXT("/Game/CtrlMethodsDemo/Input/IMC_Weapons.IMC_Weapons");
	FireMappingContext = LoadObject<UInputMappingContext>(nullptr, *MappingContextPath);

	const FString InputActionPath = TEXT("/Game/CtrlMethodsDemo/Input/Actions/IA_Shoot.IA_Shoot");
	FireAction = LoadObject<UInputAction>(nullptr, *InputActionPath);

	const FString ReloadActionPath = TEXT("/Game/CtrlMethodsDemo/Input/Actions/IA_Reload.IA_Reload");
	ReloadAction = LoadObject<UInputAction>(nullptr, *ReloadActionPath);
}


void UTP_FirearmComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (ItemBaseRef != nullptr)
	{
		if (ItemBaseRef->ItemStats.AmmoInMag > 0 && !IsReloading)
		{
			Character->GetHUD()->UpdateAmmoCounter(--ItemBaseRef->ItemStats.AmmoInMag, AmmoSumTotal);
		}
		else
		{
			return;
		}
		
	}
	
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	
	// Try and play the firing sound
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a randomized firing animation
	if (!FireAnimation.IsEmpty())
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			// Get a random index within the valid range
			int32 RandomIndex = FMath::RandRange(0, FireAnimation.Num() - 1);

			// Play the randomly selected animation
			AnimInstance->Montage_Play(FireAnimation[RandomIndex], 1.f);
		}
	}
}

void UTP_FirearmComponent::Reload()
{
	if (ItemBaseRef != nullptr)
	{
		UItemBase* AmmoItemRef = Character->GetInv()->FindItemByFNameID(ItemBaseRef->ItemAssets.UsedAmmoID);
		if (AmmoItemRef != nullptr)
		{
			CalcAmmoSumTotal();
			if (AmmoSumTotal > 0 && ItemBaseRef->ItemStats.AmmoInMag < ItemBaseRef->ItemStats.MagSize)
			{
				int AmmoToAddSum = AmmoToAdd();
				if (AmmoSumTotal >= AmmoToAddSum)
				{
					Character->GetInv()->RemoveMultipleItems(AmmoItemRef, AmmoToAddSum);
					ItemBaseRef->ItemStats.AmmoInMag += AmmoToAddSum;
				}
				else
				{
					AmmoToAddSum = AmmoSumTotal;
					Character->GetInv()->RemoveMultipleItems(AmmoItemRef, AmmoToAddSum);
					ItemBaseRef->ItemStats.AmmoInMag += AmmoToAddSum;
				}
				Character->GetHUD()->UpdateAmmoCounter(ItemBaseRef->ItemStats.AmmoInMag, AmmoSumTotal);
			}
			// Try and play the reload animation
			if (ReloadAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					IsReloading = true;
					// Play the reload animation
					FOnMontageEnded MontageEndedDelegate;
					MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
					{
						IsReloading = false; // SetIsReloading to false when the montage ends
					});

					// Play anim, and add delegate to the OnMontageEnded event
					AnimInstance->Montage_Play(ReloadAnimation, 1.f);
					AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, ReloadAnimation);
				}

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO AMMO TO RELOAD"));
		}
	}
}

void UTP_FirearmComponent::CalcAmmoSumTotal()
{
	if (Character != nullptr)
	{
		AmmoSumTotal = Character->GetInv()->OwnedItemSumByFNameID(ItemBaseRef->ItemAssets.UsedAmmoID);
		UE_LOG(LogTemp, Warning, TEXT("Ammo total recalc!")); 
	}
}

int UTP_FirearmComponent::AmmoToAdd()
{
	if (ItemBaseRef != nullptr)
	{
		return ItemBaseRef->ItemStats.MagSize-ItemBaseRef->ItemStats.AmmoInMag;
	}
	return 0;
}

void UTP_FirearmComponent::AttachWeapon(AAltControlMethodsCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no pistol yet
	if (Character == nullptr)
	{
		return;
	}
	
	if (ItemBaseRef == nullptr) {
		return;
	}

	ItemBaseRef->ItemStats.AmmoInMag = FMath::Clamp(ItemBaseRef->ItemStats.AmmoInMag, 0, ItemBaseRef->ItemStats.MagSize);
	
	FireSound = ItemBaseRef->ItemAssets.AttackSound;

	//nullptr safe anim loading based on the number of attack anims
	if (ItemBaseRef->ItemAssets.AttackAnims.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackAnims array is empty!"));
	}
	else
	{
		// Ensure FireAnimation has the same number of elements as AttackAnims
		FireAnimation.SetNum(ItemBaseRef->ItemAssets.AttackAnims.Num());
		for (int32 i = 0; i < ItemBaseRef->ItemAssets.AttackAnims.Num(); i++)
		{
			if (ItemBaseRef->ItemAssets.AttackAnims[i])
			{
				FireAnimation[i] = ItemBaseRef->ItemAssets.AttackAnims[i];
			}
		}
	}
	ReloadAnimation = ItemBaseRef->ItemAssets.ReloadAnim;

	Character->GetInv()->InvUpdate.AddUObject(this, &UTP_FirearmComponent::CalcAmmoSumTotal);
	
	//AmmoSumTotal = Character->GetInv()->OwnedItemSumByFNameID(ItemBaseRef->ItemAssets.UsedAmmoID);
	//Character->GetHUD()->UpdateAmmoCounter(ItemBaseRef->ItemStats.AmmoInMag, AmmoSumTotal);

	UE_LOG(LogTemp, Warning, TEXT("Rev Comp Attach"));
	// Attach the weapon to the First Person Character
	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_FirearmComponent::Fire);

			//Reload
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_FirearmComponent::Reload);
		}
	}
}

void UTP_FirearmComponent::DetachWeapon(AAltControlMethodsCharacter* TargetCharacter)
{
	if (TargetCharacter == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(TargetCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_FirearmComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}