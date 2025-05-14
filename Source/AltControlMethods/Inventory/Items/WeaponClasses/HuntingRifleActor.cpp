#include "AltControlMethods/Inventory/Items/WeaponClasses/HuntingRifleActor.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "WeaponMeshComps/TP_FirearmComponent.h"

AHuntingRifleActor::AHuntingRifleActor()
{
	// Projectile spawn loc
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	SkelMeshSubComp = CreateDefaultSubobject<UTP_FirearmComponent>("SkelMeshSubComp");
	SkelMeshSubComp->SetSimulatePhysics(false);
	SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(false);
	SkelMeshSubComp->SetupAttachment(SkelMeshComp);
	
}

void AHuntingRifleActor::InitWeaponActor(UItemBase* Item)
{
	Super::InitWeaponActor(Item);

	ItemBaseRef = Item;
	SkelMeshSubComp->ItemBaseRef = ItemBaseRef;
	SkelMeshSubComp->SetSkeletalMesh(ItemBaseRef->ItemAssets.SkelMesh);
	SkelMeshSubComp->SetMaterial(0, ItemBaseRef->ItemAssets.Material);
	SkelMeshSubComp->SetRelativeScale3D(ItemBaseRef->ItemAssets.Scale);
	SkelMeshSubComp->SetRelativeLocation(ItemBaseRef->ItemAssets.Position);
	SkelMeshSubComp->SetRelativeRotation(ItemBaseRef->ItemAssets.Rotation);
	
}

void AHuntingRifleActor::AttachWeapon(AAltControlMethodsCharacter* TargetCharacter)
{
	Super::AttachWeapon(TargetCharacter);

	// switch bHasRifle so the animation blueprint can switch to another animation set
	GetCharacter()->SetHasRifle(true);
	SkelMeshSubComp->AttachWeapon(GetCharacter());
}

void AHuntingRifleActor::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);

	GetCharacter()->SetHasRifle(false);
	SkelMeshSubComp->DetachWeapon(GetCharacter());
}