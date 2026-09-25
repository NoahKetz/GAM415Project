// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"

APerlinProcTerrain::APerlinProcTerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	// Creates the procedural mesh component.
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	RootComponent = ProcMesh;
}

void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();

	// Clears any old terrain data.
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UV0.Empty();

	// Creates the terrain when the game starts.
	CreateVertices();
	CreateTriangles();

	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Creates the procedural terrain mesh.
	ProcMesh->CreateMeshSection_LinearColor(
		SectionID,
		Vertices,
		Triangles,
		Normals,
		UV0,
		VertexColors,
		Tangents,
		true
	);

	// Applies the terrain material if one is selected.
	if (Material)
	{
		ProcMesh->SetMaterial(0, Material);
	}
}

void APerlinProcTerrain::CreateVertices()
{
	// Creates the vertices based on the X and Y size.
	for (int32 X = 0; X <= XSize; X++)
	{
		for (int32 Y = 0; Y <= YSize; Y++)
		{
			// Uses Perlin noise to create the terrain height.
			float Z = FMath::PerlinNoise2D(
				FVector2D(
					X * NoiseScale,
					Y * NoiseScale
				)
			) * ZMultiplier;

			Vertices.Add(
				FVector(
					X * Scale,
					Y * Scale,
					Z
				)
			);

			// Gives each vertex an upward normal.
			Normals.Add(FVector::UpVector);

			// Creates UV coordinates for the terrain material.
			UV0.Add(
				FVector2D(
					X * UVScale,
					Y * UVScale
				)
			);
		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{
	// Connects the vertices together to make triangles.
	for (int32 X = 0; X < XSize; X++)
	{
		for (int32 Y = 0; Y < YSize; Y++)
		{
			int32 Vertex0 = X * (YSize + 1) + Y;
			int32 Vertex1 = Vertex0 + 1;
			int32 Vertex2 = Vertex0 + (YSize + 1);
			int32 Vertex3 = Vertex2 + 1;

			// First triangle.
			Triangles.Add(Vertex0);
			Triangles.Add(Vertex1);
			Triangles.Add(Vertex2);

			// Second triangle.
			Triangles.Add(Vertex2);
			Triangles.Add(Vertex1);
			Triangles.Add(Vertex3);
		}
	}
}

void APerlinProcTerrain::AlterMesh(FVector ImpactPoint)
{
	// Converts the hit point to the terrain's local position.
	FVector LocalImpactPoint = ImpactPoint - GetActorLocation();

	// Checks every vertex on the terrain.
	for (int32 Index = 0; Index < Vertices.Num(); Index++)
	{
		// Changes vertices that are inside the radius.
		if (FVector::Dist(Vertices[Index], LocalImpactPoint) < Radius)
		{
			Vertices[Index].Z -= Depth;
		}
	}

	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Updates the mesh so the terrain change can be seen.
	ProcMesh->UpdateMeshSection_LinearColor(
		SectionID,
		Vertices,
		Normals,
		UV0,
		VertexColors,
		Tangents
	);
}