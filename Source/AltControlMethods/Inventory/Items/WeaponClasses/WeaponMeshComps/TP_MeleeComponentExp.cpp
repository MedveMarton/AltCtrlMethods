// Fill out your copyright notice in the Description page of Project Settings.


#include "AltControlMethods/Inventory/Items/WeaponClasses/WeaponMeshComps/TP_MeleeComponentExp.h"

#include "AltControlMethods/Inventory/Items/ItemBase.h"
#include "Kismet/GameplayStatics.h"

void UTP_MeleeComponentExp::Fire()
{
	if (GetCharacter() == nullptr || GetCharacter()->GetController() == nullptr || ItemBaseRef == nullptr)
	{
		return;
	}

	//Hitscan vectors
	FVector TraceStart{GetCharacter()->GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (GetCharacter()->GetViewRotation().Vector() * ItemBaseRef->ItemStats.Range * 100)};
	FVector DirectionVector = (TraceEnd - TraceStart).GetSafeNormal();	

	//for visible trace debugging
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Emerald, false, 1.0f, 0, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetCharacter());

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
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetCharacter()->GetActorLocation());
	}
	
	// Try and play a randomized firing animation
	if (!FireAnimation.IsEmpty() && FireAnimation.Num() == 7)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = GetCharacter()->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			LastDir = GetCharacter()->GetLastDirection();
			uint8 AnimIndex = 0;
			switch (LastDir)
			{
			case EDirection::Up_Left:
				AnimIndex = 6;
				break;
			case EDirection::Down_Left:
				AnimIndex = 5;
				break;
			case EDirection::Up_Right:
				AnimIndex = 4;
				break;
			case EDirection::Down_Right:
				AnimIndex = 3;
				break;
			case EDirection::Left:
				AnimIndex = 2;
				break;
			case EDirection::Right:
				AnimIndex = 1;
				break;
			default:
				AnimIndex = 0;
			}
			AnimInstance->Montage_Play(FireAnimation[AnimIndex], 1.f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Incompatible number of attack anims! (7 needed)"));
	}
}
