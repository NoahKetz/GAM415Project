// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinProcTerrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class GAM415PROJECT_API APerlinProcTerrain : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor.
	APerlinProcTerrain();

	// Controls the size of the terrain.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain", meta = (ClampMin = "1"))
	int32 XSize = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain", meta = (ClampMin = "1"))
	int32 YSize = 100;

	// Controls how tall the terrain can get.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float ZMultiplier = 500.0f;

	// Controls the amount of Perlin noise.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float NoiseScale = 0.1f;

	// Controls the space between vertices.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float Scale = 100.0f;

	// Controls how the material is tiled.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float UVScale = 1.0f;

	// Controls how large of an area gets changed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float Radius = 200.0f;

	// Controls how deep the terrain gets changed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float Depth = 50.0f;

	// Changes the terrain around the impact point.
	void AlterMesh(FVector ImpactPoint);

protected:
	// Called when the game starts.
	virtual void BeginPlay() override;

	// Material used on the terrain.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	UMaterialInterface* Material;

private:
	// Procedural mesh used to create the terrain.
	UPROPERTY(VisibleAnywhere, Category = "Terrain")
	UProceduralMeshComponent* ProcMesh;

	// Stores the terrain vertex locations.
	TArray<FVector> Vertices;

	// Stores the triangle indexes.
	TArray<int32> Triangles;

	// Stores the vertex normals.
	TArray<FVector> Normals;

	// Stores the UV coordinates.
	TArray<FVector2D> UV0;

	// Creates the terrain vertices.
	void CreateVertices();

	// Creates the triangles from the vertices.
	void CreateTriangles();

	int32 SectionID = 0;
};