// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UStaticMeshComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UBoxComponent;
class UMaterialInterface;
class UArrowComponent;
class AGAM415ProjectCharacter;

UCLASS()
class GAM415PROJECT_API APortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor.
	APortal();

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Portal that this portal is connected to.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	APortal* OtherPortal;

	// Render target used by the scene capture.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UTextureRenderTarget2D* RenderTarget;

	// Material used on the portal mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UMaterialInterface* Material;

protected:
	// Called when the game starts.
	virtual void BeginPlay() override;

private:
	// Detects when the player enters the portal.
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	UBoxComponent* BoxComponent;

	// Displays the portal surface.
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	UStaticMeshComponent* Mesh;

	// Captures the view from the linked portal.
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	USceneCaptureComponent2D* SceneCapture;

	// Sets where the player appears after teleporting.
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	UArrowComponent* SpawnPoint;

	// Called when something overlaps the portal.
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// Lets the player teleport again after a short delay.
	UFUNCTION()
	void SetTeleportBool(AGAM415ProjectCharacter* PlayerCharacter);

	// Updates the scene capture based on the player camera.
	void UpdatePortal();
};