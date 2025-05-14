// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/WeaponClasses/MeleeWeaponActor.h"
#include "AltControlMethods/AltControlMethodsCharacter.h"
#include "AltControlMethods/Inventory/Items/ItemBase.h"

#include "EnhancedInputSubsystems.h"

// Sets default values
AMeleeWeaponActor::AMeleeWeaponActor()
{

	PrimaryActorTick.bCanEverTick = true;
	SkelMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SkelMeshComp");
	SkelMeshComp->SetSimulatePhysics(false);
	SetRootComponent(SkelMeshComp);
	SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(false);


	SkelMeshSubComp = CreateDefaultSubobject<UTP_MeleeComponent>("SkelMeshSubComp");
	SkelMeshSubComp->SetSimulatePhysics(false);
	SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(false);
	SkelMeshSubComp->SetupAttachment(SkelMeshComp);
	
}

// Called when the game starts or when spawned
void AMeleeWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeWeaponActor::InitWeaponActor(UItemBase* ItemRef)
{
	ItemBaseRef = ItemRef;
	SkelMeshSubComp->ItemBaseRef = ItemBaseRef;
	SkelMeshSubComp->SetSkeletalMesh(ItemBaseRef->ItemAssets.SkelMesh);
	SkelMeshSubComp->SetRelativeScale3D(ItemBaseRef->ItemAssets.Scale);
	SkelMeshSubComp->SetRelativeLocation(ItemBaseRef->ItemAssets.Position);
	SkelMeshSubComp->SetRelativeRotation(ItemBaseRef->ItemAssets.Rotation);
}

void AMeleeWeaponActor::DetachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
{
	Super::DetachWeapon(AltCtrlMetChar);

	
	Character = AltCtrlMetChar;

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(DetachRules);
	this->Destroy();
}
	
void AMeleeWeaponActor::AttachWeapon(AAltControlMethodsCharacter* AltCtrlMetChar)
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