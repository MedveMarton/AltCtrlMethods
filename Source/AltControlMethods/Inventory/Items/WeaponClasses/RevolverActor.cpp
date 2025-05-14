// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/WeaponClasses/RevolverActor.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "AltControlMethods/Inventory/Items/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponMeshComps/TP_FirearmComponent.h"

ARevolverActor::ARevolverActor()
{
	// Projectile spawn loc
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	SkelMeshSubComp = CreateDefaultSubobject<UTP_FirearmComponent>("SkelMeshSubComp");
	SkelMeshSubComp->SetSimulatePhysics(false);
	SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(false);
	SkelMeshSubComp->SetupAttachment(SkelMeshComp);
	
	//ProjectileClass = LoadObject<AProjectileBase>();
	//FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/FPWeapons/Audio/GShot.GShot"));
	//FireAnimation = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/FirstPersonArms/Animations/FP_Pistol_Shoot_Montage.FP_Pistol_Shoot_Montage"));
}

void ARevolverActor::InitWeaponActor(UItemBase* Item)
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

void ARevolverActor::AttachWeapon(AAltControlMethodsCharacter* TargetCharacter)
{
	Super::AttachWeapon(TargetCharacter);

	// switch bHasPistol so the animation blueprint can switch to another animation set
	GetCharacter()->SetHasPistol(true);
	SkelMeshSubComp->AttachWeapon(GetCharacter());
}

void ARevolverActor::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);

	GetCharacter()->SetHasPistol(false);
	SkelMeshSubComp->DetachWeapon(GetCharacter());
}