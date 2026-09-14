// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAM415ProjectProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS(config = Game)
class AGAM415ProjectProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** Visible projectile mesh created through C++ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BallMesh;


	UPROPERTY(EditDefaultsOnly, Category = Materials)
	UMaterialInterface* ProjectileMaterial;

	/** Material used to create the splatter decal */
	UPROPERTY(EditDefaultsOnly, Category = Materials)
	UMaterialInterface* SplatMaterial;


	UPROPERTY()
	UMaterialInstanceDynamic* ProjectileDMI;

	/** Stores one random color so the projectile and decal can share it */
	FLinearColor RandColor;

protected:

	
	virtual void BeginPlay() override;

public:

	AGAM415ProjectProjectile();

	/** Called when projectile hits something */
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	/** Returns CollisionComp subobject */
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject */
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};