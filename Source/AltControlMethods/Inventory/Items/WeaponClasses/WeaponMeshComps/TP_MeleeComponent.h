#pragma once

#include "CoreMinimal.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_MeleeComponent.generated.h"

UCLASS()
class ALTCONTROLMETHODS_API UTP_MeleeComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	/** Sets default values for this component's properties */
	UTP_MeleeComponent();

	UPROPERTY()
	UItemBase* ItemBaseRef;

	AAltControlMethodsCharacter* GetCharacter() const
	{
		return this->Character;
	}

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<UAnimMontage*> FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AAltControlMethodsCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DetachWeapon(AAltControlMethodsCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	UPROPERTY()
	AAltControlMethodsCharacter* Character;
};
