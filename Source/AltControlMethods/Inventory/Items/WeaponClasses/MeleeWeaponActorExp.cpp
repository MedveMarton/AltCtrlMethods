// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/WeaponClasses/MeleeWeaponActorExp.h"

#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "WeaponMeshComps/TP_MeleeComponentExp.h"

AMeleeWeaponActorExp::AMeleeWeaponActorExp()
{

	PrimaryActorTick.bCanEverTick = true;
	SkelMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SkelMeshComp");
	SkelMeshComp->SetSimulatePhysics(false);
	SetRootComponent(SkelMeshComp);
	SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(false);


	SkelMeshSubComp = CreateDefaultSubobject<UTP_MeleeComponentExp>("SkelMeshSubComp");
	SkelMeshSubComp->SetSimulatePhysics(false);
	SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(false);
	SkelMeshSubComp->SetupAttachment(SkelMeshComp);
	
}

void AMeleeWeaponActorExp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeWeaponActorExp::InitWeaponActor(UItemBase* ItemRef)
{
	ItemBaseRef = ItemRef;
	SkelMeshSubComp->ItemBaseRef = ItemBaseRef;
	SkelMeshSubComp->SetSkeletalMesh(ItemBaseRef->ItemAssets.SkelMesh);
	SkelMeshSubComp->SetRelativeScale3D(ItemBaseRef->ItemAssets.Scale);
	SkelMeshSubComp->SetRelativeLocation(ItemBaseRef->ItemAssets.Position);
	SkelMeshSubComp->SetRelativeRotation(ItemBaseRef->ItemAssets.Rotation);
}

void AMeleeWeaponActorExp::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);

	
	Character = AltCtrlMetChar;

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(DetachRules);
	this->Destroy();
}

void AMeleeWeaponActorExp::AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Character = AltCtrlMetChar;

	UE_LOG(LogTemp, Warning, TEXT("Melee Attach method is called!"));

	// Check that the character is valid
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
}
