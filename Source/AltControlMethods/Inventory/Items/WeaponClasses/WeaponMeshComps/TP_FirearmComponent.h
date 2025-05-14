#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_FirearmComponent.generated.h"

class UItemBase;
class AAltControlMethodsCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALTCONTROLMETHODS_API UTP_FirearmComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	/** Sets default values for this component's properties */
	UTP_FirearmComponent();

	UPROPERTY()
	UItemBase* ItemBaseRef;

	UPROPERTY()
	int AmmoSumTotal;

	UPROPERTY()
	bool IsReloading;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadAnimation;
	
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	//class UInputMappingContext* ReloadMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;
	
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AAltControlMethodsCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DetachWeapon(AAltControlMethodsCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void CalcAmmoSumTotal();

	//returns how much ammo is added into the mag considering max magsize
	UFUNCTION(BlueprintCallable, Category="Weapon")
	int AmmoToAdd();
	
protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	UPROPERTY()
	AAltControlMethodsCharacter* Character;
};
