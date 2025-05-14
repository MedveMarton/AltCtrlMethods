#pragma once

#include "CoreMinimal.h"
#include "Data/EDirection.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "Inventory/Items/WeaponActorBase.h"
#include "Logging/LogMacros.h"
#include "AltControlMethodsCharacter.generated.h"

class UItemBase;
class UInventoryComponent;
class AAltCtrlMetHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipAnimChangeEvent);

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()
	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};
	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AAltControlMethodsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AimingAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ToggleMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PauseGameAction;
	
public:
	AAltControlMethodsCharacter();
			
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
		
	//** WeaponActorBase reference for equipping and un-equipping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	AWeaponActorBase* MainWepEquipped;

	//** WeaponActorBase reference for equipping and un-equipping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UItemBase* EquippedItemBase;

	UPROPERTY(BlueprintAssignable)
	FEquipAnimChangeEvent OnEquipAnimEvent;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasPistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasOhSword;

	/** Movement and action bools */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	bool bIsAiming;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasPistol(bool bNewHasPistol);
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasSword(bool bNewHasSword);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasOhSword(bool bNewHasSword);
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasSword() const;

	

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


	void DropItem(UItemBase* Item, const int32 Quantity);

	void EquipRangedItem(UItemBase* Item);

	bool IsInteracting() const
	{
		return GetWorldTimerManager().IsTimerActive(TimerHandle_Inter);
	};

	UInventoryComponent* GetInv() const
	{
		return UserInv;
	};

	void UpdateInterWidget() const;

protected:
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* UserInv;
	
	UPROPERTY()
	AAltCtrlMetHUD* HUD;

public:
	[[nodiscard]] AAltCtrlMetHUD* GetHUD() const
	{
		return HUD;
	}

protected:
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFreq;

	float InteractionCheckDist;

	float LookDirThreshold;

	EDirection LastDirection;

public:
	[[nodiscard]] EDirection GetLastDirection() const
	{
		return LastDirection;
	}

protected:
	FTimerHandle TimerHandle_Inter;

	FInteractionData InteractionData;

	static FString DirectionToString(EDirection Direction);

	void DoInteractionCheck();
	void FoundInteractable(AActor* NewInterable);
	void BeginInteract();
	void EndInteract();
	void Interact();
	void NoInteractableFound();
	
	void StartCrouch();
	void EndCrouch();
	void StartSprint();
	void EndSprint();
	void StartAiming();
	void EndAiming();

	void ToggleMenu();
	void HideInvIfVis();
	
	virtual void Tick(float DeltaSeconds) override;
};