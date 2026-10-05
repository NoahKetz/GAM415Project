// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAM415ProjectCharacter.h"
#include "GAM415ProjectProjectile.h"
#include "TimerManager.h"


APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	// Creates the collision area for the portal.
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	BoxComponent->SetBoxExtent(FVector(32.0f, 100.0f, 200.0f));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Allows players and projectiles to overlap the portal.
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Creates the visible portal mesh.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Creates the camera that displays the linked portal.
	SceneCapture =
		CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));

	SceneCapture->SetupAttachment(Mesh);

	// Creates the point where actors appear after teleporting.
	SpawnPoint =
		CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));

	SpawnPoint->SetupAttachment(BoxComponent);
}


void APortal::BeginPlay()
{
	Super::BeginPlay();

	// Calls the overlap function when something enters the portal.
	BoxComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&APortal::OnOverlapBegin
	);

	// Applies the portal material.
	if (Material)
	{
		Mesh->SetMaterial(0, Material);
	}

	// Sends the scene capture image to the render target.
	if (RenderTarget)
	{
		SceneCapture->TextureTarget = RenderTarget;
	}

	// Keeps the portal mesh from appearing inside its own capture.
	Mesh->SetHiddenInSceneCapture(true);
	Mesh->SetCastShadow(false);
}


void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Updates the portal view every frame.
	UpdatePortal();
}


void APortal::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || !OtherPortal)
	{
		return;
	}

	// Checks if a projectile entered the portal.
	AGAM415ProjectProjectile* Projectile =
		Cast<AGAM415ProjectProjectile>(OtherActor);

	if (Projectile)
	{
		UProjectileMovementComponent* ProjectileMovement =
			Projectile->GetProjectileMovement();

		if (ProjectileMovement)
		{
			// Saves the projectile's current speed.
			float ProjectileSpeed =
				ProjectileMovement->Velocity.Size();

			// Gets the destination portal direction.
			FRotator ProjectileRotation =
				OtherPortal->SpawnPoint->GetComponentRotation();

			// Moves the projectile to the other portal.
			Projectile->SetActorLocationAndRotation(
				OtherPortal->SpawnPoint->GetComponentLocation(),
				ProjectileRotation,
				false,
				nullptr,
				ETeleportType::TeleportPhysics
			);

			// Sends the projectile forward out of the destination portal.
			ProjectileMovement->Velocity =
				OtherPortal->SpawnPoint->GetForwardVector() *
				ProjectileSpeed;

			ProjectileMovement->UpdateComponentVelocity();
		}

		return;
	}

	// Checks if the player entered the portal.
	AGAM415ProjectCharacter* PlayerCharacter =
		Cast<AGAM415ProjectCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		if (!PlayerCharacter->isTeleporting)
		{
			PlayerCharacter->isTeleporting = true;

			// Gets the destination spawn point.
			FVector TeleportLocation =
				OtherPortal->SpawnPoint->GetComponentLocation();

			// Keeps the player's capsule above the floor.
			if (PlayerCharacter->GetCapsuleComponent())
			{
				TeleportLocation.Z +=
					PlayerCharacter->GetCapsuleComponent()
					->GetScaledCapsuleHalfHeight();
			}

			// Gets the forward direction of the destination portal.
			FRotator TeleportRotation =
				OtherPortal->SpawnPoint->GetComponentRotation();

			TeleportRotation.Pitch = 0.0f;
			TeleportRotation.Roll = 0.0f;

			// Moves and rotates the player at the destination.
			PlayerCharacter->SetActorLocationAndRotation(
				TeleportLocation,
				TeleportRotation,
				false,
				nullptr,
				ETeleportType::TeleportPhysics
			);

			// Makes the camera face forward after teleporting.
			if (PlayerCharacter->GetController())
			{
				PlayerCharacter->GetController()->SetControlRotation(
					TeleportRotation
				);
			}

			// Waits before allowing another teleport.
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;

			TimerDelegate.BindUFunction(
				this,
				FName("SetTeleportBool"),
				PlayerCharacter
			);

			GetWorldTimerManager().SetTimer(
				TimerHandle,
				TimerDelegate,
				1.0f,
				false
			);
		}
	}
}


void APortal::SetTeleportBool(
	AGAM415ProjectCharacter* PlayerCharacter
)
{
	if (PlayerCharacter)
	{
		// Allows the player to use a portal again.
		PlayerCharacter->isTeleporting = false;
	}
}


void APortal::UpdatePortal()
{
	if (!OtherPortal || !SceneCapture)
	{
		return;
	}

	APlayerCameraManager* CameraManager =
		UGameplayStatics::GetPlayerCameraManager(
			GetWorld(),
			0
		);

	if (!CameraManager)
	{
		return;
	}

	// Gets the distance between both portals.
	FVector PortalDifference =
		GetActorLocation() -
		OtherPortal->GetActorLocation();

	// Gets the player's current camera location and rotation.
	FVector CameraLocation =
		CameraManager->GetCameraLocation();

	FRotator CameraRotation =
		CameraManager->GetCameraRotation();

	// Moves the scene capture based on the player's camera.
	FVector CombinedLocation =
		CameraLocation + PortalDifference;

	SceneCapture->SetWorldLocationAndRotation(
		CombinedLocation,
		CameraRotation
	);
}