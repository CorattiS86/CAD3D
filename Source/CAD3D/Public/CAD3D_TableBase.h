// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAD3D_TableBase.generated.h"

class UProceduralMeshComponent;

UCLASS()
class CAD3D_API ACAD3D_TableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAD3D_TableBase();

    FORCEINLINE FVector GetLocalBoundsMax() const { return LocalBoundsMax; }
    void UpdateLocalBoundsMax(FVector NewLocalBoundsMax);

    FORCEINLINE FVector GetLocalBoundsMin() const { return LocalBoundsMin; }
    void UpdateLocalBoundsMin(FVector NewLocalBoundsMin);

protected:
    virtual void Update();
	 
protected:

    const float TableSizeLowerLimitX = 100.0f;
    const float TableSizeLowerLimitY = 100.0f;
    const float TableSizeLowerLimitZ = 10.0f;

    const float TableHeight = 65.0f;

    const int32 NumLegs = 4;
    TArray<UStaticMeshComponent*> TableLegMeshes;
    FVector TableLegMeshOrigin;
    FVector TableLegMeshExtent;

public:
  
    static const FString LegMeshPath;
    static const FString TableAndChairMaterialPath;

    static FVector GetLegMeshOrigin();
    static FVector GetLegMeshExtent();


private:
    USceneComponent* TableOriginComponent;

    FVector LocalBoundsMax;
    FVector LocalBoundsMin;

    UProceduralMeshComponent* TableTopMesh;

    struct VertexData
    {
        int32 NumVertices;
        TArray<FVector> VertexPositions;
        TArray<FVector> VertexNormals;
        TArray<FVector2D> VertexUVs;
        TArray<int32> Triangles;

    } TableTopMeshVertexData;

    void CreateTableTopMesh();
    void UpdateTableTopMesh();

    void UpdateTableLegLocations();
};
