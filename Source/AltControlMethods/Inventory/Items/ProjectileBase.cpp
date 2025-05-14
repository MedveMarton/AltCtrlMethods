
#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AProjectileBase::AProjectileBase() 
{
	//Sphere collision
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(2.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit); //Notification for when this component hits sth

	//Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	//Set as root component
	RootComponent = CollisionComp;
	
	BMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BMeshComponent"));
	BMeshComponent->SetupAttachment(CollisionComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BMeshAsset(TEXT("/AltControlMethods/Content/Models/Mesh.Projectile"));
	if (BMeshAsset.Succeeded())
	{
		BMeshComponent->SetStaticMesh(BMeshAsset.Object);
		BMeshComponent->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
		BMeshComponent->SetRelativeLocation(FVector(0, 0, 0));
		BMeshComponent->SetRelativeRotation(FRotator(0, 0, 0));
	}

	//Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 9000.f;
	ProjectileMovement->MaxSpeed = 9000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	//Destroy after 6 secs
	InitialLifeSpan = 6.0f;
}

void AProjectileBase::InitMeleeProjectile()
{
	BMeshComponent->SetStaticMesh(nullptr);
	ProjectileMovement->InitialSpeed = 700.f;
	ProjectileMovement->MaxSpeed = 900.f;
	SetLifeSpan(0.2f);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}
