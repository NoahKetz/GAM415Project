// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415ProjectProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"


AGAM415ProjectProjectile::AGAM415ProjectProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGAM415ProjectProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(
		FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f)
	);

	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	// Visible projectile mesh created through C++.
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(CollisionComp);
	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}


void AGAM415ProjectProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Generate one color that will be shared by the projectile and decal.
	RandColor = FLinearColor(
		FMath::FRandRange(0.0f, 1.0f),
		FMath::FRandRange(0.0f, 1.0f),
		FMath::FRandRange(0.0f, 1.0f),
		1.0f
	);

	if (ProjectileMaterial && BallMesh)
	{
		ProjectileDMI =
			UMaterialInstanceDynamic::Create(ProjectileMaterial, this);

		if (ProjectileDMI)
		{
			BallMesh->SetMaterial(0, ProjectileDMI);

			ProjectileDMI->SetVectorParameterValue(
				TEXT("ProjColor"),
				RandColor
			);
		}
	}
}


void AGAM415ProjectProjectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	if ((OtherActor != nullptr) &&
		(OtherActor != this) &&
		(OtherComp != nullptr))
	{
		if (SplatMaterial)
		{
			// Randomly chooses one of the four splat frames.
			const float FrameNumber =
				static_cast<float>(FMath::RandRange(0, 3));

			UDecalComponent* Decal =
				UGameplayStatics::SpawnDecalAtLocation(
					GetWorld(),
					SplatMaterial,
					FVector(20.0f, 40.0f, 40.0f),
					Hit.ImpactPoint + Hit.ImpactNormal * 1.0f,
					Hit.ImpactNormal.Rotation(),
					0.0f
				);

			if (Decal)
			{
				UMaterialInstanceDynamic* DecalDMI =
					Decal->CreateDynamicMaterialInstance();

				if (DecalDMI)
				{
					// Matches the decal color to the projectile color.
					DecalDMI->SetVectorParameterValue(
						TEXT("Color"),
						RandColor
					);

					DecalDMI->SetScalarParameterValue(
						TEXT("Frame"),
						FrameNumber
					);
				}
			}
		}

		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(
				GetVelocity() * 100.0f,
				GetActorLocation()
			);
		}

		Destroy();
	}
}