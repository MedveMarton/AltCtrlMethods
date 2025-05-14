#include "AltControlMethods/Inventory/Items/WeaponClasses/WeaponMeshComps/TP_MeleeComponent.h"

#include "EnhancedInputSubsystems.h"
#include "AltcontrolMethods/AltControlMethodsCharacter.h"
#include "AltcontrolMethods/Inventory/Items/ProjectileBase.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"

UTP_MeleeComponent::UTP_MeleeComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(65.0f, 5.0f, 15.0f);

	/*
	Only used when the melee weapons still used invisible projectiles
	static ConstructorHelpers::FClassFinder<AProjectileBase> ProjectileBPClass(TEXT("/Game/CtrlMethodsDemo/Blueprints/BP_ProjectileBase.BP_ProjectileBase_C"));
	if (ProjectileBPClass.Succeeded())
	{
		ProjectileClass = ProjectileBPClass.Class;
	}
	*/

	FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/FPWeapons/Audio/Swing.Swing"));

	/*
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageFinder(TEXT("/Game/FirstPersonArms/Animations/FP_Sword_Hit_Montage.FP_Sword_Hit_Montage"));
	if (AnimMontageFinder.Succeeded())
	{
		FireAnimation = AnimMontageFinder.Object;
	};
	*/
	
	const FString MappingContextPath = TEXT("/Game/CtrlMethodsDemo/Input/IMC_Weapons.IMC_Weapons");
	FireMappingContext = LoadObject<UInputMappingContext>(nullptr, *MappingContextPath);

	const FString InputActionPath = TEXT("/Game/CtrlMethodsDemo/Input/Actions/IA_Shoot.IA_Shoot");
	FireAction = LoadObject<UInputAction>(nullptr, *InputActionPath);
}

void UTP_MeleeComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr || ItemBaseRef == nullptr)
	{
		return;
	}

	//Hitscan vectors
	FVector TraceStart{Character->GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (Character->GetViewRotation().Vector() * ItemBaseRef->ItemStats.Range * 100)};
	FVector DirectionVector = (TraceEnd - TraceStart).GetSafeNormal();	

	//for visible trace debugging
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Emerald, false, 1.0f, 0, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	if (FHitResult TraceHit; GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor())
		{
			UPrimitiveComponent* PrimitiveComp = TraceHit.GetActor()->FindComponentByClass<UPrimitiveComponent>();

			if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics()){
				PrimitiveComp->AddImpulseAtLocation(DirectionVector * 100000.0f, TraceHit.ImpactPoint);
			}
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

void UTP_MeleeComponent::AttachWeapon(AAltControlMethodsCharacter* TargetCharacter)
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
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_MeleeComponent::Fire);
		}
	}
}

void UTP_MeleeComponent::DetachWeapon(AAltControlMethodsCharacter* TargetCharacter)
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

void UTP_MeleeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
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