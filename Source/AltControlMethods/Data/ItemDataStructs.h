#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

class UActorFactoryClass;

UENUM()
enum class EItemType : uint8
{
	General UMETA(DisplayName = "General"), RangedWeapon UMETA(DisplayName = "RangedWeapon"), MeleeWeapon UMETA(DisplayName = "MeleeWeapon"), Consumable UMETA(DisplayName = "Consumable"), Ammunition UMETA(DisplayName = "Ammunition")
};

USTRUCT()
struct FItemStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	float Range;
	
	UPROPERTY(EditAnywhere)
	int MagSize;

	UPROPERTY(EditAnywhere)
	int AmmoInMag;
	
	UPROPERTY(EditAnywhere)
	float Healing;
};

USTRUCT()
struct FItemText
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	FText Desc;
	
	UPROPERTY(EditAnywhere)
	FText InterText;
	
	UPROPERTY(EditAnywhere)
	FText InvUseText;

	UPROPERTY(EditAnywhere)
	FText InvAltUseText;
};

USTRUCT()
struct FInvItemStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;
	
	UPROPERTY(EditAnywhere)
	int32 StackSize;
	
	UPROPERTY()
	bool bIsStackable;
	
};

USTRUCT()
struct FItemAssets
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* InvIcon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkelMesh;

	UPROPERTY(EditAnywhere)
	UMaterial* Material;

	UPROPERTY(EditAnywhere)
	FVector Scale;

	UPROPERTY(EditAnywhere)
	FVector Position;

	UPROPERTY(EditAnywhere)
	FRotator Rotation;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackAnims;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere)
	FName UsedAmmoID;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStats ItemStats;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FInvItemStats InvItemStats;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemText ItemText;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssets ItemAssets;
};
